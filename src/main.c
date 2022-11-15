#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

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

    pthread_t thread_id_1, thread_id_2;
    pthread_create(&thread_id_1, NULL, non_blocking_get, (void *) 1);
    pthread_create(&thread_id_2, NULL, blocking_get, (void *) 1);

    sleep(3);

    printf("put key=1, val=%d\n", v);
    kkv_put(1, (void *) &v, sizeof(int));

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    kkv_destroy();

    exit(0);
}