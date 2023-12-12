// read_data.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>

#define SHM_SIZE 1024

// 具有跨进程能力的互斥锁
pthread_mutex_t *mutex;

// 共享内存数据
int *shared_data;

void init_shared_memory() {
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, SHM_SIZE);

    // 映射共享内存
    shared_data = (int*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // 映射具有跨进程能力的互斥锁
    mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // 初始化具有跨进程能力的互斥锁
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutex_attr);
}


void cleanup_shared_memory() {
    // 解除映射
    munmap(mutex, sizeof(pthread_mutex_t));
    munmap(shared_data, SHM_SIZE);

    // 关闭并删除共享内存对象
    shm_unlink("/my_shared_memory");
}

void read_data() {
    char data[30] = {0};
    while (1) {
        // 读操作前获取互斥锁
        //pthread_mutex_lock(mutex);

        // 读取数据
        printf("Read Process: %d\n", *shared_data);
        //memcpy(&data, shared_data, sizeof(data));
        //printf("get data: %d\n", data);

        // 读操作结束后释放互斥锁
        //pthread_mutex_unlock(mutex);

        // 休眠，模拟读操作的时间间隔
        usleep(500000);  // 500ms
    }
}

int main() {
    // 初始化共享内存和具有跨进程能力的互斥锁
    init_shared_memory();

    // 创建读数据的线程
    read_data();

    // 清理共享内存和具有跨进程能力的互斥锁
    cleanup_shared_memory();

    return 0;
}
