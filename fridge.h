#ifndef _FRIDGE_H_
#define _FRIDGE_H_

#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define HASH_TABLE_LENGTH 17
#define KKV_NONBLOCK 0
#define KKV_BLOCK 1

#define min(a, b) a < b ? a : b

long kkv_init();
long kkv_destroy();
long kkv_put(int key, void *val, size_t size);
long kkv_get(int key, void *val, size_t size, int flags);

/*
 * A key-value pair.
 */
struct kkv_pair {
	void *val;
	int key;
	size_t size;
};

/*
 * A node in a linked list. Contains a key-value pair.
 */
struct kkv_ht_entry {
	struct list_head entries;
	struct kkv_pair kv_pair;
	sem_t sem;
	int q_count;
};

/*
 * A bucket in the hash table.
 * The hash table is an array of HASH_TABLE_LENGTH buckets.
 */
struct kkv_ht_bucket {
	pthread_spinlock_t lock;
	struct list_head entries;
	int count;
};

#endif
