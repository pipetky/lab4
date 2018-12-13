#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#define BUFFER_SIZE	64

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

void * file_read_th(int *fd) {

    ssize_t read_bytes;
    char buffer[BUFFER_SIZE+1];
    while ((read_bytes = read (*fd, buffer, BUFFER_SIZE)) > 0)
    {
        buffer[read_bytes] = 0; /* Null-terminator for C-string */
        fputs (buffer, stdout);
    }

    if (read_bytes < 0)
    {
        fprintf (stderr, "myread: Cannot read file\n");
        exit (1);
    }
    close (*fd);
    printf("\nHello from thread!\n");

    return SUCCESS;
}




int main() {
    int fd = 0;

    fd = open ("file.txt", O_RDONLY);
    if (fd < 0)
    {
        fprintf (stderr, "openclose: Cannot open file '%s'\n",
                "file.txt");
        exit (1);
    }

    pthread_t thread;
    int status;

    status = pthread_create(&thread, NULL, file_read_th, &fd);
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