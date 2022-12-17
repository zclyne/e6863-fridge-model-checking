#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify kkv_put and kkv_get returns -EPERM if they are called when the store is not initialized yet
int main()
{
    int v = 3, get_v;
    long ret;

    kkv_init();
    ret = kkv_put(1, (void *)&v, sizeof(int));
    __CPROVER_assert(ret == 0, "kkv_put successfully puts value");
    kkv_destroy();
}
