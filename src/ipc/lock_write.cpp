// write_data.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
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

    // 初始化共享内存中的数据
    //*shared_data = 0;
    memset(shared_data, 0, SHM_SIZE);
}

void cleanup_shared_memory() {
    // 销毁具有跨进程能力的互斥锁
    pthread_mutex_destroy(mutex);

    // 解除映射
    munmap(mutex, sizeof(pthread_mutex_t));
    munmap(shared_data, SHM_SIZE);

    // 关闭并删除共享内存对象
    shm_unlink("/my_shared_memory");
}

void write_data() {
    std::string data = "test_data";
    for(int i = 0; i<1000;++i) {
        // 写操作前获取互斥锁
        //pthread_mutex_lock(mutex);

        // 模拟写数据
        *shared_data = i;
        printf("write data: %d\n", *shared_data);

        //(*shared_data) = i;
        //printf("write data: %d\n", *shared_data);
        //std::string send_data = data + std::to_string(i);
        //memcpy(shared_data, send_data.c_str(), send_data.size());
        //printf("Writing data %s\n", (char*)shared_data);


        // 写操作结束后释放互斥锁
        //pthread_mutex_unlock(mutex);

        // 休眠，模拟写操作的时间间隔
        usleep(500000);  // 500ms
    }
}

int main() {
    // 初始化共享内存和具有跨进程能力的互斥锁
    init_shared_memory();

    // 创建写数据的线程
    write_data();

    // 清理共享内存和具有跨进程能力的互斥锁
    cleanup_shared_memory();

    return 0;
}
