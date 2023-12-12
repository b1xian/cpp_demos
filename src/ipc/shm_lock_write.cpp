#include "shm_lock_common.cpp"

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


int main() {
    signal(SIGINT, sigint_handler);

    std::vector<std::string> img_list = GetImgList();
    printf("img_list size : %ld\n", img_list.size());
    cv::Mat mat = cv::imread(img_list[0]);
    size_t img_data_size = mat.cols * mat.rows * mat.channels();
    printf("img_data_size : %ld\n", img_data_size);
    //for (int i = 0; i < img_list.size(); ++i) {
    //    //mat = cv::imread(img_list[i]);
    //    size_t startPos = img_list[i].find_last_of('/') + 1;
    //    size_t endPos = img_list[i].find_last_of('.');
    //    std::string timestamp = img_list[i].substr(startPos, endPos - startPos);
    //    uint64_t time = atol(timestamp.c_str());
    //    printf("read img : %s, %s %ld\n", img_list[i].c_str(), timestamp.c_str(), time);
    //}

    // 创建mutex
    pthread_mutex_t *mutex;
    int res = InitMutex(mutex_key_name, &mutex);
    if (res == -1) {
        return res;
    }
    printf("mutex: %p\n", mutex);
    printf("-- init mutex success\n");

    void *shm_addr;
    size_t total_size = img_data_size + sizeof(uint64_t) + sizeof(int);
    int shmId;
    res = InitShmMem(shm_key_name, proj_id, &shm_addr, total_size, shmId);
    if (res == -1) {
        return res;
    }
    printf("-- InitShmMem success\n");


    //创建需写入内存的数据并将其写入共享内存区
    size_t i = 0;
    int camera_mask = 15;
    while(loop) {
        printf("read img : %s\n", img_list[i].c_str());
        mat = cv::imread(img_list[i]);
        size_t startPos = img_list[i].find_last_of('/') + 1;
        size_t endPos = img_list[i].find_last_of('.');
        std::string timestamp = img_list[i].substr(startPos, endPos - startPos);
        uint64_t time = atol(timestamp.c_str());
        auto start = std::chrono::system_clock::now();

        pthread_mutex_lock(mutex);
        std::memcpy(shm_addr, &camera_mask, sizeof(camera_mask));
        std::memcpy((char*)shm_addr + sizeof(camera_mask), &time, sizeof(time));
        char* frame_data = (char*)shm_addr + sizeof(camera_mask) + sizeof(time);
        std::memcpy(frame_data, mat.data, img_data_size);
        auto end = std::chrono::system_clock::now();
        // 写操作结束后释放互斥锁
        pthread_mutex_unlock(mutex);

        double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
        printf("Writing %d time cost : %f ms\n", i, time_cost);

        // 休眠，模拟写操作的时间间隔
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //usleep(500000);  // 500ms
        i++;
    }

    printf("Writing is finish ! clear memory\n");
    pthread_mutex_lock(mutex);
    memset(shm_addr, 0, total_size);
    // 写操作结束后释放互斥锁
    pthread_mutex_unlock(mutex);


    //断开
    shmdt(shm_addr);

    // 写入程序中不需要释放共享内存（第4步），否则读取程序无法正确读取数据
    //将共享内存释放
    int sh = shmctl(shmId, IPC_RMID, NULL);
    if (sh == -1) {
        perror("shmctl error");
        return -1;
    }
    printf("shmctl is worked. release shm memory\n");
    return 0;

}
