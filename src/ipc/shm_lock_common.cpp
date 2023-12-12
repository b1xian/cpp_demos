#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include<opencv2/opencv.hpp>
#include <chrono>
#include "./sem.hpp"

#include <dirent.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <signal.h>


const std::string mutex_key_name = "/vision_miivii_frame_mutex_5";
const std::string shm_key_name = "/home";
const int proj_id = 0x5;

std::vector<std::string> GetImgList() {
    std::string img_path = "/home/sheldon/work_space/data/vision/evaluate_data/track_metrics/proj_imgs/scene5_3_2022-06-13-13-47-39.bag";
    std::vector<std::string> img_list;
    struct dirent *dirp;
    DIR* dir = opendir(img_path.c_str());
    while ((dirp = readdir(dir)) != nullptr) {
        if (dirp->d_type != DT_REG) { continue; }
        img_list.push_back(img_path + "/" + dirp->d_name);
    }
    std::sort(img_list.begin(), img_list.end(),
        [](const std::string& a, const std::string& b) {
                size_t startPos = a.find_last_of('/') + 1;
                size_t endPos = a.find_last_of('.');
                std::string timestamp = a.substr(startPos, endPos - startPos);
                uint64_t a_time = atol(timestamp.c_str());
                startPos = b.find_last_of('/') + 1;
                endPos = b.find_last_of('.');
                timestamp = b.substr(startPos, endPos - startPos);
                uint64_t b_time = atol(timestamp.c_str());
            return a_time < b_time;
        });
    return img_list;
}

int InitMutex(const std::string& key_name, pthread_mutex_t **mutex) {
    int mutex_fd = shm_open(key_name.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (mutex_fd == -1) {
        perror("shm_open error");
        return mutex_fd;
    }
    int res = ftruncate(mutex_fd, sizeof(pthread_mutex_t));
    if (res == -1) {
        perror("ftruncate error");
        return res;
    }
    // 映射pthread_mutex_t的共享内存
    *mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_fd, 0);
    // 初始化具有跨进程能力的互斥锁
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(*mutex, &mutex_attr);
    if (*mutex == nullptr) {
        perror("init mutex failed!!!");
        return -1;
    }
    return 0;
}

int InitShmMem(const std::string& key_name, int proj_id, void** shm_addr, size_t size, int& shmId) {
    /**********实现步骤中的第1步**********/
    //利用ftok获得一个IPC键值，
    key_t key = ftok(key_name.c_str(), proj_id);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }
    printf("ftok key: %d\n", key);

    //获取共享内存区域的ID
    shmId = shmget(key, size , IPC_CREAT | 0600);
    if (shmId == -1) {
        perror("shmget error");
        return -1;
    }
    printf("shmget id: %d\n", shmId);

    /**********实现步骤中的第2步**********/
    //连接共享内存
    *shm_addr = shmat(shmId, NULL, 0);
    return 0;
}

