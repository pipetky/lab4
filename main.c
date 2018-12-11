#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

void* helloWorld(int args) {
    struct sched_param b;
    int a = sched_getparam(getpid(), &b);
    printf("Hello from thread!\n");
    int c = sched_get_priority_max();
    printf("%d", a);
    printf("%d\n", args);
    return SUCCESS;
}

int main() {
    FILE *f = fopen("file.txt", "r");
    pthread_t thread;
    int status;
    int status_addr;

    status = pthread_create(&thread, NULL, helloWorld(f), NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
    printf("Hello from main!\n");

    status = pthread_join(thread, (void **) &status_addr);
    if (status != SUCCESS) {
        printf("main error: can't join thread, status = %d\n", status);
        exit(ERROR_JOIN_THREAD);
    }

    printf("joined with address %d\n", status_addr);
    return 0;
}