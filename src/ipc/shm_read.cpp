#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <thread>
#include<opencv2/opencv.hpp>
#include <chrono>
#include "sem.hpp"

int main() {
    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
    cv::imwrite("./src.jpg", mat);

    size_t img_data_size = mat.cols * mat.rows * mat.channels();
    char* read_data = new char [img_data_size];
    printf("img_data_size : %ld\n", img_data_size);
    const char* key_name = "/home";
    /**********实现步骤中的第1步**********/
    //ftok的第一个参数必须保证和写入程序中一致，最好别写相对路径
    key_t key = ftok(key_name, 0x666);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }
    printf("ftok : %d\n", key);

    int shmId = shmget(key, img_data_size, IPC_CREAT | 0600);
    if (shmId == -1) {
        perror("shmget error");
        return -1;
    }
    printf("shmget : %d\n", shmId);

    /**********实现步骤中的第2步**********/
    void *shm_addr = shmat(shmId, NULL, 0);

    int semId = GetSem("test_img_sem");
    if (semId == -1) {
        perror("GetSem error");
    }

    //取出数据
    //char data[30] = {0};
    //memcpy(&data, shm_addr, sizeof(data));
    //std::cout << "get data : " << data << std::endl;
    //for (int i = 0; i < 10; ++i) {
    //    memcpy(&data, shm_addr, sizeof(data));
    //    std::cout << "get data : " << data << std::endl;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //}
    auto start = std::chrono::system_clock::now();
    memcpy(read_data, shm_addr, img_data_size);
    auto end = std::chrono::system_clock::now();
    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
    printf("read time cost : %f ms\n", time_cost);
    cv::Mat read_mat(mat.rows, mat.cols, CV_8UC3, read_data);
    cv::imwrite("./read.jpg", read_mat);

    /**********实现步骤中的第3步**********/
    //断开
    shmdt(shm_addr);

    /**********实现步骤中的第4步**********/
    //将共享内存释放
    int sh = shmctl(shmId, IPC_RMID, NULL);
    if (sh == -1) {
        perror("shmctl error");
        return -1;
    }
    printf("shmctl is worked.\n");

    return 0;
}
