#include <sys/syscall.h>
#include <unistd.h>
#define gettid() syscall(__NR_gettid)

#include <thread>
#include <pthread.h>
#include <signal.h>


//void* child_thread1(void *arg) {
//    int i;
//    for(i=0;i<5;i++) {
//        printf("I'm child_thread1 !\n");
//        std::this_thread::sleep_for(std::chrono::seconds(10));
//    }
//}
//
//void* child_thread2(void *arg) {
//    int i;
//    for(i=0;i<5;i++) {
//        printf("I'm child_thread2 !\n");
//        std::this_thread::sleep_for(std::chrono::seconds(10));
//    }
//}
//
//void* thread(void *arg) {
//    pthread_t id1,id2;
//    pthread_create(&id1,NULL, child_thread1,NULL);
//    pthread_create(&id2,NULL, child_thread2,NULL);
//
//    pthread_join(id1,NULL);
//    pthread_join(id2,NULL);
//
//}
//void func2();
bool loop = true;
void sigint_handler(int sig) {
    if (sig == SIGINT) {
        printf("get SIGINT...\n");
        loop = false;
    } else if (sig == SIGKILL) {
        printf("get SIGKILL...\n");
        loop = false;
    } else if (sig == SIGTERM) {
        printf("get SIGTERM...\n");
        loop = false;
    } else if (sig == SIGSTOP) {
        printf("get SIGSTOP...\n");
        loop = false;
    }
}

int main()
{
    //pthread_t id;
    //pthread_create(&id,NULL,thread,NULL);
    //printf("main: %d", id);
    //pthread_join(id,NULL);
    signal(SIGINT, sigint_handler);
    printf("main: %d", gettid());
    while (loop) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;

}

