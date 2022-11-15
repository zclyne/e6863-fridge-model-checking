/*
 * fridge.c
 *
 * A kernel-level key-value store. Accessed via user-defined
 * system calls. This is the module implementation.
 */

#include <errno.h>

#include "fridge.h"

// hash computes the hash value of the given key
// by simply mod the length of the hash table
int hash(int key)
{
	return key % HASH_TABLE_LENGTH;
}

static struct kkv_ht_bucket **hash_table;
static pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER;
static int destroyed;

int is_inited(void)
{
	return hash_table ? 1 : 0;
}

// initialize the fridge k-v store
long kkv_init()
{
	int i;

	pthread_rwlock_wrlock(&rw_lock);
	if (is_inited()) {
		pthread_rwlock_unlock(&rw_lock);
		return -EPERM;
	}
	destroyed = 0;
	hash_table = malloc(HASH_TABLE_LENGTH * sizeof(struct kkv_ht_bucket *));
	if (!hash_table) {
		pthread_rwlock_unlock(&rw_lock);
		return -EFAULT;
	}

	for (i = 0; i < HASH_TABLE_LENGTH; i++) {
		hash_table[i] = malloc(sizeof(struct kkv_ht_bucket));
        pthread_spin_init(&hash_table[i]->lock, 0);
		INIT_LIST_HEAD(&hash_table[i]->entries);
	}

	pthread_rwlock_unlock(&rw_lock);
	return 0;
}

// destroy the fridge k-v store
long kkv_destroy()
{
	int i, count;
	struct kkv_ht_bucket *bucket;
	struct kkv_ht_entry *ht_entry, *next_ht_entry;

	count = 0;
	pthread_rwlock_wrlock(&rw_lock);
	destroyed = 1;

	if (!is_inited()) {
		pthread_rwlock_unlock(&rw_lock);
		return -EPERM;
	}

	// spin lock
	for (i = 0; i < HASH_TABLE_LENGTH; i++) {
		bucket = hash_table[i];
		pthread_spin_lock(&bucket->lock);
		// for each entry in the bucket, remove it from the linked list and free the memory
		list_for_each_entry_safe(ht_entry, next_ht_entry, &bucket->entries, entries) {
			// increment count only if it is not a fake entry
			if (ht_entry->q_count == 0)
				count++;
			// loop until all GETs blocked on the entry returns
			while (ht_entry->q_count > 0) {
				pthread_spin_unlock(&bucket->lock);
				pthread_rwlock_unlock(&rw_lock);
				sem_post(&ht_entry->sem);
				pthread_rwlock_wrlock(&rw_lock);
				pthread_spin_lock(&bucket->lock);
			}
			list_del(&ht_entry->entries);
            sem_destroy(&ht_entry->sem);
			free(ht_entry->kv_pair.val);
            free(ht_entry);
		}
		free(bucket);
	}
	free(hash_table);
	hash_table = NULL;
	pthread_rwlock_unlock(&rw_lock);

	return count;
}

long kkv_put(int key, void *val, size_t size)
{
	bool found;
	struct kkv_ht_bucket *bucket;
	struct kkv_ht_entry *ht_entry, *new_entry;
	void *buffer;
    bool wake_up = false;
	int sem_value;

	pthread_rwlock_rdlock(&rw_lock);
	if (!is_inited()) {
		pthread_rwlock_unlock(&rw_lock);
		return -EPERM;
	}

	if (val == NULL) {
		pthread_rwlock_unlock(&rw_lock);
		return -EINVAL;
	}

	// pre allocate the memory and copy the val
	// so that malloc() is not called while holding the spin lock
	buffer = malloc(size);
	if (!buffer) {
		pthread_rwlock_unlock(&rw_lock);
		return -ENOMEM;
	}
    memcpy(buffer, val, size);

    // initialize the new entry
	new_entry = malloc(sizeof(struct kkv_ht_entry));
	if (!new_entry) {
		free(buffer);
		pthread_rwlock_unlock(&rw_lock);
		return -ENOMEM;
	}
	new_entry->kv_pair.key = key;
	new_entry->kv_pair.size = size;
	new_entry->kv_pair.val = buffer;
	new_entry->q_count = 0;
    // the entry has a valid value, so the semaphore is initialized to 1
    sem_init(&new_entry->sem, 0, 1);
	INIT_LIST_HEAD(&new_entry->entries);

	found = false;
	bucket = hash_table[hash(key)];

	pthread_spin_lock(&bucket->lock);

	// check whether the given key already exists
	list_for_each_entry(ht_entry, &bucket->entries, entries) {
		if (ht_entry->kv_pair.key == key) {
			found = true;
			break;
		}
	}

	// printf("found=%d\n", found);

	if (found) { // key already exists
		// update the value and size of the existing kv pair
		// val may be NULL because the entry may be a dummy one created by a blocking GET
		free(ht_entry->kv_pair.val);
		free(new_entry);
		ht_entry->kv_pair.val = buffer;
		ht_entry->kv_pair.size = size;
		// if some GETs are blocked on the entry, wake up one
		sem_getvalue(&ht_entry->sem, &sem_value);
		// printf("sem_value=%d\n", sem_value);
		if (sem_value == 0) {
			// printf("gonna wake up a blocked get\n");
			// bucket->count needs to be incremented here because ht_entry is a fake one
			bucket->count++;
			wake_up = true;
		}
	} else { // key doesn't exist
		// add the new entry to the tail of the list
		list_add(&new_entry->entries, &bucket->entries);
		bucket->count++;
	}

	pthread_spin_unlock(&bucket->lock);
	pthread_rwlock_unlock(&rw_lock);

    // wake up a GET blocked on the entry
	if (wake_up)
		sem_post(&ht_entry->sem);
	return 0;
}

long kkv_get(int key, void *val, size_t size, int flags)
{

	size_t actual_size;
	long ret;
	struct kkv_ht_entry *ht_entry, *next_ht_entry, *new_entry;
	struct kkv_ht_bucket *bucket;
	struct kkv_pair *pair;
	void *buffer;

	pthread_rwlock_rdlock(&rw_lock);
	if (!is_inited()) {
		pthread_rwlock_unlock(&rw_lock);
		return -EPERM;
	}

	if (val == NULL) {
		pthread_rwlock_unlock(&rw_lock);
		return -EINVAL;
	}

	ret = -ENOENT;
	bucket = hash_table[hash(key)];

	new_entry = malloc(sizeof(struct kkv_ht_entry));

	pthread_spin_lock(&bucket->lock);
	list_for_each_entry_safe(ht_entry, next_ht_entry, &bucket->entries, entries) {
		if (ht_entry->kv_pair.key == key) {
			// fake entry
			if (ht_entry->q_count > 0)
				ret = 1;
			// find
			else
				ret = 0;
			break;
		}
	}

	// kv pair not found or fake entry, should block
	if (ret != 0 && flags) {
		if (ret == 1) // fake entry
			free(new_entry);
		else if (ret == -ENOENT) { // No entry, create a fake one
			// printf("no entry found, gonna create a fake one\n");
			new_entry->kv_pair.key = key;
			new_entry->q_count = 0;
            // no value available, initialized to 0
			sem_init(&new_entry->sem, 0, 0);
			INIT_LIST_HEAD(&new_entry->entries);
			list_add(&new_entry->entries, &bucket->entries);
			// should not increment count here because the entry is fake
			// bucket->count++;
			ht_entry = new_entry;
		}
		++ht_entry->q_count;

        // block
		pthread_spin_unlock(&bucket->lock);
        sem_wait(&ht_entry->sem);
		pthread_spin_lock(&bucket->lock);

		--ht_entry->q_count;
		if (destroyed) {
			free(buffer);
			pthread_spin_unlock(&bucket->lock);
			pthread_rwlock_unlock(&rw_lock);
			return -EPERM;
		}
		ret = 0; // found the kv pair
	}

	// successfully found the kv pair
	if (ret == 0) {
		pair = &ht_entry->kv_pair;
		// copy the value of the kv pair to the user
		actual_size = min(pair->size, size);
		memcpy(val, pair->val, actual_size);
	    free(pair->val);
		pair->val = NULL;
		bucket->count--;
		if (ht_entry->q_count == 0) {
            // cut the entry off the linked list
			list_del(&ht_entry->entries);
            sem_destroy(&ht_entry->sem);
            free(ht_entry);
        }
	}
    pthread_spin_unlock(&bucket->lock);
	pthread_rwlock_unlock(&rw_lock);
	return ret;
}
