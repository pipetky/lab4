#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>


#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

void * file_read_th(FILE * file) {
    char str[1024];
    while (fscanf(file, "%s", str) != EOF)
    {
        printf("%s\n",str);

    }
    fclose(file);



    printf("Hello from thread!\n");

    return SUCCESS;
}




int main() {
    FILE *f = fopen("file.txt", "r");
    pthread_t thread;
    int status;


    status = pthread_create(&thread, NULL, file_read_th, f);
    if (status != SUCCESS) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }
    printf("Hello from main!\n");

    status = pthread_join(thread, NULL);

    if (status != SUCCESS) {
        printf("main error: can't join thread, status = %d\n", status);
        exit(ERROR_JOIN_THREAD);
    }

    printf("joined");
    return 0;
}