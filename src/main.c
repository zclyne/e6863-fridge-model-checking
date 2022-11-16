#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "fridge.h"

void *non_blocking_get(void *vargp)
{
    int key = (int) vargp;
    long ret;
    int val;

    ret = kkv_get(key, &val, sizeof(int), KKV_NONBLOCK);

    if (ret == -ENOENT) {
        printf("no entry found for key=%d\n", key);
    } else if (ret == 0) {
        printf("nonblocking get ret=%ld, val=%d\n", ret, val);
    }

    return NULL;
}

void *blocking_get(void *vargp)
{
    int key = (int) vargp;
    long ret;
    int val;

    ret = kkv_get(key, &val, sizeof(int), KKV_BLOCK);

    printf("blocking get ret=%ld, val=%d\n", ret, val);

    return NULL;
}

int main() {
    long ret;

    kkv_init();

    int v = 3;

    pthread_t thread_id_1, thread_id_2, thread_id_3;
    pthread_create(&thread_id_1, NULL, blocking_get, (void *) 1);
    pthread_create(&thread_id_2, NULL, blocking_get, (void *) 1);
    pthread_create(&thread_id_3, NULL, blocking_get, (void *) 1);
    sleep(1); // make sure all the kkv_gets are blocked
    printf("nums blocked=%d\n", num_gets_blocked(1));
    ret = kkv_put(1, (void *) &v, sizeof(int));
    printf("put ret = %ld\n", ret);
    sleep(1); // give the blocked gets a chance to run
    // terminate the threads
    printf("nums blocked=%d\n", num_gets_blocked(1));
    pthread_cancel(thread_id_1);
    pthread_cancel(thread_id_2);
    pthread_cancel(thread_id_3);

    printf("put key=1, val=%d\n", v);
    kkv_put(1, (void *) &v, sizeof(int));

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    kkv_destroy();

    exit(0);
}