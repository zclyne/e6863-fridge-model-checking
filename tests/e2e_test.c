#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>

#include "../src/fridge.h"

int test_put_override_old_value() {
    int val = 3, get_val;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    ret = kkv_get(1, (void *) &get_val, sizeof(int), KKV_NONBLOCK);
    assert(ret == 0 && get_val == 3);
    val = 5;
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    ret = kkv_get(1, (void *) &get_val, sizeof(int), KKV_NONBLOCK);
    assert(ret == 0 && get_val == 5);
    kkv_destroy();
    return 0;
}

void *blocking_get(void *vargp)
{
    int key = (int) vargp;
    long ret;
    int val;

    ret = kkv_get(key, &val, sizeof(int), KKV_BLOCK);
    pthread_exit(val);
}

int test_get_blocked() {
    int val = 3;
    void *get_val;
    long ret;

    kkv_init();
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, blocking_get, (void *) 1);
    sleep(1); // make sure the get is blocked
    assert(num_gets_blocked(1) == 1);
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    sleep(1); // give the blocked gets a chance to run
    pthread_join(thread_id, &get_val);
    assert((int) get_val == 3);
    
    kkv_destroy();
    return 0;
}

// test_put_wakes_up_one_get tests the scenario where
// multiple gets are blocked on the same key
// and one put operation can only wake up one get
int test_put_wakes_up_one_get() {
    int val = 3, get_val;
    long ret;

    kkv_init();
    // make 3 blocking get calls
    pthread_t thread_id_1, thread_id_2, thread_id_3;
    pthread_create(&thread_id_1, NULL, blocking_get, (void *) 1);
    pthread_create(&thread_id_2, NULL, blocking_get, (void *) 1);
    pthread_create(&thread_id_3, NULL, blocking_get, (void *) 1);
    sleep(1); // make sure all the kkv_gets are blocked

    assert(num_gets_blocked(1) == 3);
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    sleep(1); // give the blocked gets a chance to run
    assert(num_gets_blocked(1) == 2);
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    sleep(1);
    assert(num_gets_blocked(1) == 1);
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    sleep(1);
    // the last get removes the entry
    assert(num_gets_blocked(1) == -ENOENT);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);
    pthread_join(thread_id_3, NULL);

    kkv_destroy();
    return 0;
}



int main() {
    printf("test_put_override_old_value...\n");
    test_put_override_old_value();

    printf("test_get_blocked...\n");
    test_get_blocked();
    
    printf("test_put_wakes_up_one_get...\n");
    test_put_wakes_up_one_get();

    printf("all end-to-end tests passed\n");
    exit(0);
}