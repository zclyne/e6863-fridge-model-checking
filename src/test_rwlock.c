#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *myThreadFun(void *vargp)
{
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

int main() {
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    pthread_join(thread_id, NULL);
    printf("After Thread\n");

    exit(0);
}