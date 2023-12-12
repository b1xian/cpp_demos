// write_data.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <string>
#include <string.h>
#include<opencv2/opencv.hpp>
//
//#define SHM_SIZE 1024
//
//// 具有跨进程能力的互斥锁
//pthread_mutex_t *mutex;
//
//// 共享内存数据
//void *shared_data;
//
//void init_shared_memory(size_t img_data_size) {
//    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
//    ftruncate(shm_fd, img_data_size);
//
//    // 映射共享内存
//    shared_data = mmap(NULL, img_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
//    memset(shared_data, 0, img_data_size);
//}
//
//void cleanup_shared_memory(size_t img_data_size) {
//    munmap(shared_data, img_data_size);
//    // 关闭并删除共享内存对象
//    shm_unlink("/my_shared_memory");
//}
//
//void write_data() {
//
//}
//
//int main() {
//    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
//    size_t img_data_size = mat.cols * mat.rows * mat.channels();
//
//    // 初始化共享内存和具有跨进程能力的互斥锁
//    init_shared_memory(img_data_size);
//
//    auto start = std::chrono::system_clock::now();
//    memcpy(shared_data, mat.data, img_data_size);
//    auto end = std::chrono::system_clock::now();
//    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
//    printf("Writing time cost : %f ms\n", time_cost);
//    printf("Writing is finish !\n");
//
//    // 清理共享内存和具有跨进程能力的互斥锁
//    while (1) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
//    //cleanup_shared_memory(img_data_size);
//    return 0;
//}

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#define SHM_SIZE 1024

//int main() {
//    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
//    size_t img_data_size = mat.cols * mat.rows * mat.channels();
//
//    // 创建文件，作为共享内存对象
//    int fd = open("shared_memory_example", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
//    ftruncate(fd, img_data_size);
//
//    // 映射共享内存
//    void* shared_memory = mmap(NULL, img_data_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
//
//    close(fd); // 不再需要文件描述符
//
//    // 写入数据到共享内存
//    auto start = std::chrono::system_clock::now();
//    memcpy(shared_memory, mat.data, img_data_size);
//    auto end = std::chrono::system_clock::now();
//    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
//    printf("Writing time cost : %f ms\n", time_cost);
//    printf("Writing is finish !\n");
//
//    // 解除内存映射
//    munmap(shared_memory, img_data_size);
//    shm_unlink("shared_memory_example");
//
//    return 0;
//}

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const char* shm_name = "/my_shared_memory";
    size_t shm_size = 1024;

    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    ftruncate(shm_fd, shm_size);

    void* shm_addr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shm_addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 在共享内存中写入数据
    const char* message = "Hello from write process!";
    snprintf(static_cast<char*>(shm_addr), shm_size, "%s", message);

    // 解除映射
    munmap(shm_addr, shm_size);

    // 关闭共享内存对象
    close(shm_fd);

    return 0;
}
