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

    int pid = getpid();

    // schedule policy
    int policy = sched_getscheduler(pid);

    // current priority
    int priority = getpriority(PRIO_PROCESS, pid);

    int max_priority = sched_get_priority_max(policy);
    int min_priority = sched_get_priority_min(policy);

    switch (policy) {
        case 0:
            printf("Schedule class: %s\n", "SCHED_FIFO");
            break;
        case 1:
            printf("Schedule class: %s\n", "SCHED_RR");
            break;
        case 2:
            printf("Schedule class: %s\n", "SCHED_OTHER");
            break;
    }
    printf("Current priority: %i\n", priority);
    printf("Max priority: %i\n", max_priority);
    printf("Min priority: %i\n", min_priority);

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
    printf("joined\n");
    if((fcntl (fd, F_GETFL, NULL)) < 0 )
    {
        printf("file closed\n");

    } else
    {
        printf("\n"
               "the file was not closed, closing the file...\n");
        close(fd);
    }


    return 0;
}