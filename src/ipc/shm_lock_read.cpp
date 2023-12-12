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

    cv::Mat mat = cv::imread("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/src.jpg");
    size_t img_data_size = mat.cols * mat.rows * mat.channels();
    char* read_data = new char [img_data_size + sizeof(uint64_t) + sizeof(int)];
    printf("img_data_size : %ld\n", img_data_size);

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

    int camera_mask = 15;
    int read_camera_mask = 0;
    uint64_t time = 0;
    uint64_t last_time = 0;
    while (loop) {
        auto start = std::chrono::system_clock::now();
        pthread_mutex_lock(mutex);
        // 读取相机掩码
        std::memcpy(&read_camera_mask, shm_addr, sizeof(camera_mask));
        //printf("read camera mask : %d\n", read_camera_mask);
        // 读取时间戳
        std::memcpy(&time, (char*)shm_addr + sizeof(camera_mask), sizeof(time));
        if (read_camera_mask != camera_mask) {
            printf("no camera data, waiting!!!\n");
            pthread_mutex_unlock(mutex);
            // 休眠，模拟写操作的时间间隔
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        if (last_time != 0 && time <= last_time) {
            printf("frame not update, continue\n");
            pthread_mutex_unlock(mutex);
            // 休眠，模拟写操作的时间间隔
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        std::memcpy(read_data, shm_addr, total_size);
        pthread_mutex_unlock(mutex);
        auto end = std::chrono::system_clock::now();
        double time_cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;
        cv::Mat read_mat = cv::Mat(mat.rows, mat.cols, CV_8UC3, read_data + sizeof(read_camera_mask) + sizeof(time));
        printf("read camera mask : %d, read time : %ld, time cost : %f ms\n", camera_mask, time, time_cost);
        cv::imwrite("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/ipc/test_write_1_scene5_3/read_"
                        + std::to_string(time)+".jpg", read_mat);
        last_time = time;
        // 休眠，模拟写操作的时间间隔
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }




    /**********实现步骤中的第3步**********/
    //断开
    shmdt(shm_addr);
    /**********实现步骤中的第4步**********/
    //将共享内存释放
    //int sh = shmctl(shmId, IPC_RMID, NULL);
    //if (sh == -1) {
    //    perror("shmctl error");
    //    return -1;
    //}
    //printf("shmctl is worked. release shm memory\n");

    return 0;
}
