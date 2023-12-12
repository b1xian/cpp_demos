
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void checkResults(char *string, int rc) {
   if (rc) {
     printf("Error on : %s, rc=%d", string, rc);
     exit(EXIT_FAILURE);
   }
   return;
}

void *threadfunc(void *parm) {
    printf("Entered secondary thread\n");
    while (1) {
        printf("Secondary thread is looping\n");
        pthread_testcancel();
        sleep(1);
    }
}

int main(int argc, char **argv) {
    pthread_t             thread;
    int                   rc=0;

    printf("Entering testcase\n");

    /* Create a thread using default attributes */
    printf("Create thread using the NULL attributes\n");
    rc = pthread_create(&thread, NULL, threadfunc, NULL);
    checkResults("pthread_create(NULL)\n", rc);

    /* sleep() is not a very robust way to wait for the thread */
    sleep(1);

    //printf("Cancel the thread\n");
    //rc = pthread_cancel(thread);
    //checkResults("pthread_cancel()\n", rc);

    /* sleep() is not a very robust way to wait for the thread */
    sleep(10);
    printf("Main completed\n");
    return 0;
}
