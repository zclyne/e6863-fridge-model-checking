#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify that for non-blocking kkv_get operations, if there is no such key value pair in the store, return an error code
int main()
{
    int get_v;
    long ret;

    kkv_init();
    ret = kkv_get(1, (void *)&get_v, sizeof(int), 0);
    __CPROVER_assert(ret == -ENOENT, "kkv_get gets not existing k-v pair");
    kkv_destroy();
}
