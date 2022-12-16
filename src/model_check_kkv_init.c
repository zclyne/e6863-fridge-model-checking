#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "fridge.h"
#include "fridge.c"

int main()
{
    kkv_init();
}
