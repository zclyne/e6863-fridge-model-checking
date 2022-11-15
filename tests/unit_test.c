#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "../src/fridge.h"

int test_init() {
    long ret;

    ret = kkv_init();
    assert(ret == 0 && is_inited() == 1);
    kkv_destroy();
    return 0;
}

int test_destroy() {
    long ret;

    ret = kkv_init();
    assert(ret == 0 && is_inited() == 1);
    ret = kkv_destroy();
    assert(ret == 0 && is_inited() == 0);
    return 0;
}

int test_put_success() {
    int val = 3;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    kkv_destroy();
    return 0;
}

int test_put_uninitialized() {
    int val = 3;
    long ret;

    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == -EPERM);
    return 0;
}

int test_put_invalid_value() {
    long ret;

    kkv_init();
    ret = kkv_put(1, NULL, sizeof(int));
    assert(ret == -EINVAL);
    kkv_destroy();
    return 0;
}

int test_get_success() {
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

int test_get_uninitialized() {
    int val = 3, get_val;
    long ret;

    ret = kkv_get(1, (void *) &get_val, sizeof(int), KKV_NONBLOCK);
    assert(ret = -EPERM);
    return 0;
}

int test_get_value_null() {
    int val = 3;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    ret = kkv_get(1, NULL, sizeof(int), KKV_NONBLOCK);
    assert(ret == -EINVAL);
    kkv_destroy();
    return 0;
}

int test_get_nonblocking_no_entry() {
    int val = 3, get_val;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *) &val, sizeof(int));
    assert(ret == 0);
    ret = kkv_get(2, (void *) &get_val, sizeof(int), KKV_NONBLOCK);
    assert(ret == -ENOENT);
    kkv_destroy();
    return 0;
}

void *blocking_get(void *vargp)
{
    int key = (int) vargp;
    long ret;
    int val;

    ret = kkv_get(key, &val, sizeof(int), KKV_BLOCK);

    return &val;
}

int main() {
    printf("test_init...\n");
    test_init();
    printf("test_destroy...\n");
    test_destroy();

    printf("test_put_success...\n");
    test_put_success();
    printf("test_put_uninitialized...\n");
    test_put_uninitialized();
    printf("test_put_invalid_value...\n");
    test_put_invalid_value();

    printf("test_get_success...\n");
    test_get_success();
    printf("test_get_uninitialized...\n");
    test_get_uninitialized();
    printf("test_get_value_null...\n");
    test_get_value_null();
    printf("test_get_nonblocking_no_entry...\n");
    test_get_nonblocking_no_entry();

    printf("all unit tests passed\n");
    exit(0);
}

