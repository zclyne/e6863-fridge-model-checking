#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify that for kkv_put and kkv_get, an error code will be returned if value is null
int main()
{
    long ret;
    
    kkv_init();
    ret = kkv_put(1, (void *)NULL, sizeof(int));
    __CPROVER_assert(ret == -EINVAL, "kkv_put inputs null value");
    ret = kkv_get(1, (void *)NULL, sizeof(int), 0);
    __CPROVER_assert(ret == -EINVAL, "kkv_get inputs null value");
    kkv_destroy();
}
