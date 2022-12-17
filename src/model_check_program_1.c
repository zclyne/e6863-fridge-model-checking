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
    int v = 3;
    long ret = kkv_put(1, (void *)&v, sizeof(int));
    __CPROVER_assert(ret == -EPERM, "store not initialized. kkv_put returns -EPERM");
    ret = kkv_get(1, (void *)&v, sizeof(int), 0);
    __CPROVER_assert(ret == -EPERM, "store not initialized. kkv_get returns -EPERM");
}
