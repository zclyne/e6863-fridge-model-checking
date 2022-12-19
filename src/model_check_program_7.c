#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify that if multiple kkv_put calls are made, the last one overrides the value of previous ones
int main()
{
    int v = 3, get_v;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *)&v, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    v = 4;
    ret = kkv_put(1, (void *)&v, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    ret = kkv_get(1, (void *)&get_v, sizeof(int), 0);
    __CPROVER_assert(ret == 0 && get_v == 4, "kkv_get successfully gets the value put by latest kkv_put");
    kkv_destroy();
}
