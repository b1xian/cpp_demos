#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include<opencv2/opencv.hpp>
#include <chrono>
#include "./sem.hpp"


int main() {
    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
    size_t img_data_size = mat.cols * mat.rows * mat.channels();
    printf("img_data_size : %ld\n", img_data_size);

    const char* key_name = "/home";
    /**********实现步骤中的第1步**********/
    //利用ftok获得一个IPC键值，
    key_t key = ftok(key_name, 0x666);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }
    printf("ftok : %d\n", key);

    //获取共享内存区域的ID
    //img_data_size = 4096;
    int shmId = shmget(key, img_data_size, IPC_CREAT | 0600);
    if (shmId == -1) {
        perror("shmget error");
        return -1;
    }
    printf("shmget : %d\n", shmId);

    /**********实现步骤中的第2步**********/
    //连接共享内存
    void *shm_addr = shmat(shmId, NULL, 0);

    // 获取信号量
    int semId = CreateSem("test_img_sem", 0);
    if (semId == -1) {
        perror("CreateSem: err");
        return -1;
    }
    // 将要写入数据，semId置为1，占用资源
    if (SemV(semId) == -1) {
        perror("SemV w error:");
        return 0;
    }
    printf("set sem 1, write data\n");


    //创建需写入内存的数据并将其写入共享内存区
    std::string data = "test_data";
    //memcpy(shm_addr, data.c_str(), img_data_size);
    //for (int i = 0; i < 10; ++i) {
    //    std::string send_data = data + std::to_string(i);
    //    memcpy(shm_addr, send_data.c_str(), sizeof(data));
    //    printf("Writing data %s\n", send_data.c_str());
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //}
    auto start = std::chrono::system_clock::now();
    memcpy(shm_addr, mat.data, img_data_size);
    auto end = std::chrono::system_clock::now();
    double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
    printf("Writing time cost : %f ms\n", time_cost);
    printf("Writing is finish !\n");

    // 数据写完了，semId置为0，其余进程可以来获取数据
    if (SemP(semId) == -1) {
        perror("SemP w error:");
        return 0;
    }
    printf("set sem 0, you can get data\n");

    /**********实现步骤中的第3步**********/
    //断开
    shmdt(shm_addr);

    // 写入程序中不需要释放共享内存（第4步），否则读取程序无法正确读取数据

    return 0;

}
