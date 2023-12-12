#include <iostream>
#include <vector>

void func1(void)
{
    int *x = (int*)malloc(8 * sizeof(int));
    x[9] = 0; //数组下标越界
} //内存未释放


int func2(void)
{
    char *p = (char*)malloc(1); //分配
    *p = 'a';

    char c = *p;

    printf("\n [%c]\n",c);

    free(p); //释放
    c = *p; //取值
    return 0;
}

int func3(void)
{
    char *p = (char*)malloc(1); //分配1字节
    *p = 'a';
    char c = *(p+1); //地址加1
    printf("\n [%c]\n",c);
    free(p);
    return 0;
}

int func4(void)
{
    char *p;
    p=(char *)malloc(100);
    if(p)
        printf("Memory Allocated at: %s/n",p);
    else
        printf("Not Enough Memory!/n");
    free(p); //重复释放
    free(p);
    free(p);
    return 0;
}

struct CameraFrame {
    uint64_t frame_id;         // 图像帧id
    uint64_t frame_stamp = 0; // 图像曝光时间 nsec
    uint64_t recv_stamp = 0; // 接收图像时间 nsec
    uint8_t camera_mark = 0; // camera标志位(如0xff:所有8个camera开启)
    //cv::Mat frame_mats[CAMERA_NUM]; // mat数组，仅标志位为1的mat非空
    std::vector<int> frame_mats;
};

int main() {
    std::cout << "valgrind" << std::endl;
    //func1();
    //func2();
    //func3();
    func4();
    return 0;
}
