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
#include <chrono>
#include <string.h>
#include<opencv2/opencv.hpp>
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
//
//void cleanup_shared_memory(size_t img_data_size) {
//    // 解除映射
//    munmap(mutex, sizeof(pthread_mutex_t));
//    munmap(shared_data, img_data_size);
//
//    // 关闭并删除共享内存对象
//    shm_unlink("/my_shared_memory");
//}
//
//int main() {
//    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
//    size_t img_data_size = mat.cols * mat.rows * mat.channels();
//    char* read_data = new char [img_data_size];
//
//    // 初始化共享内存和具有跨进程能力的互斥锁
//    init_shared_memory(img_data_size);
//
//    auto start = std::chrono::system_clock::now();
//    memcpy(read_data, shared_data, img_data_size);
//    auto end = std::chrono::system_clock::now();
//    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
//    printf("read time cost : %f ms\n", time_cost);
//    cv::Mat read_mat(mat.rows, mat.cols, CV_8UC3, read_data);
//    cv::imwrite("./read.jpg", read_mat);
//
//
//    // 清理共享内存和具有跨进程能力的互斥锁
//    cleanup_shared_memory(img_data_size);
//
//    return 0;
//}



#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_SIZE 1024

//int main() {
//
//    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
//    size_t img_data_size = mat.cols * mat.rows * mat.channels();
//    char* read_data = new char [img_data_size];
//
//    // 打开已存在的文件，作为共享内存对象
//    int fd = open("shared_memory_example", O_RDWR);
//    if (fd == -1) {
//        std::cerr << "Error opening file" << std::endl;
//        return 1;
//    }
//
//    // 映射共享内存
//    void* shared_memory = mmap(NULL, img_data_size, PROT_READ, MAP_SHARED, fd, 0);
//
//
//    auto start = std::chrono::system_clock::now();
//    memcpy(read_data, shared_memory, img_data_size);
//    auto end = std::chrono::system_clock::now();
//    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
//    printf("read time cost : %f ms\n", time_cost);
//    cv::Mat read_mat(mat.rows, mat.cols, CV_8UC3, read_data);
//    cv::imwrite("./mmap_read.jpg", read_mat);
//
//    // 解除内存映射
//    munmap(shared_memory, img_data_size);
//
//    // 关闭文件描述符
//    close(fd);
//    shm_unlink("shared_memory_example");
//
//    return 0;
//}


#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
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

    // 在共享内存中读取数据
    std::cout << "Read Process: " << static_cast<char*>(shm_addr) << std::endl;

    // 解除映射
    munmap(shm_addr, shm_size);

    // 关闭共享内存对象
    close(shm_fd);

    return 0;
}

