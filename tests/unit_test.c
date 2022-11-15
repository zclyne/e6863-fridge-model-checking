#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "../src/fridge.h"

int test_init() {
    kkv_init();
    assert(is_inited() == 1);
    kkv_destroy();
    return 0;
}

int test_destroy() {
    kkv_init();
    assert(is_inited() == 1);
    kkv_destroy();
    assert(is_inited() == 0);
    return 0;
}

int test_put() {
    int val = 3;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    kkv_destroy();
    return 0;
}

int test_get() {
    int val = 3, get_val;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    ret = kkv_get(1, (void *) &get_val, sizeof(int), KKV_NONBLOCK);
    assert(get_val == 3);
    kkv_destroy();
    return 0;
}

int main() {
    printf("test_init...\n");
    test_init();
    printf("test_destroy...\n");
    test_destroy();
    printf("test put...\n");
    test_put();
    printf("test get...\n");
    test_get();

    printf("all unit tests passed\n");
}

