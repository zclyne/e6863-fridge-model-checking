#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify kkv_get retrieves correct value from Fridge and removes the key-value pair from Fridge
int main()
{
    int v = 3, get_v;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *)&v, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    ret = kkv_get(1, (void *)&get_v, sizeof(int), 0);
    __CPROVER_assert(ret == 0 && get_v == 3, "kkv_get successfully gets the value put by kkv_put");
    ret = kkv_get(1, (void *)&get_v, sizeof(int), 0);
    __CPROVER_assert(ret == -ENOENT, "kkv_get successfully removes the entry from store");
    kkv_destroy();
}
