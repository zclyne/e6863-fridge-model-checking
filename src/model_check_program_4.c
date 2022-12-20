#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

// verify Fridge can be successfully initialized and destroyed
int main()
{
    int v = 3, get_v;
    long ret;

    ret = kkv_init();
    __CPROVER_assert(ret == 0, "kkv_init succeeded");
    ret = kkv_destroy();
    __CPROVER_assert(ret >= 0, "kkv_destroy succeeded");
}
