#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify multiple kkv_puts with same key. only the last one takes effect
int main()
{
    int v1 = 1, v2 = 2, v3 = 3, get_v;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *)&v1, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    ret = kkv_put(1, (void *)&v2, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    ret = kkv_put(1, (void *)&v3, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    ret = kkv_get(1, (void *)&get_v, sizeof(int), 0);
    __CPROVER_assert(ret == 0 && get_v == 3, "kkv_get successfully gets the value put by the last kkv_put");
    kkv_destroy();
}
