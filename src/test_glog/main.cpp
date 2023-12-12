//
// Created by sheldon on 2022/3/24.
//
#include <thread>

#include "vision_log.h"

void thread_log(const std::string logger_name) {

    for (int i = 0; i < 30000; ++i) {
        LOG(INFO) << "thread " << logger_name << "value " << i;
    }
}

void thread_basic_logfile_example() {
    std::thread camera1_thread(thread_log, "camera1");
    std::thread camera2_thread(thread_log, "camera2");
    std::thread camera3_thread(thread_log, "camera3");
    std::thread camera4_thread(thread_log, "camera4");
    std::thread camera5_thread(thread_log, "camera5");
    std::thread camera6_thread(thread_log, "camera6");
    std::thread camera7_thread(thread_log, "camera7");
    std::thread camera8_thread(thread_log, "camera8");
    std::thread camera9_thread(thread_log, "camera9");
    std::thread camera10_thread(thread_log, "camera10");
    camera1_thread.join();
    camera2_thread.join();
    camera3_thread.join();
    camera4_thread.join();
    camera5_thread.join();
    camera6_thread.join();
    camera7_thread.join();
    camera8_thread.join();
    camera9_thread.join();
    camera10_thread.join();
}

void test_backtrace () {
    //for(int i = 0 ; i < 10000; ++i) {
    //    LOG(WARNING) << "i~~~~~~~~~~~~" << i;
    //}
    //float *f;
    //f[1] = 0.;
    //f[2] = 0.;
    //printf("%.2f\n", f[3]);

    std::vector<int> vec;
    vec.push_back(1);
    int v2 = vec.at(2);
}

int main (int argc, char** argv) {
    /// initial glog
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter([](const char* ch, int i) {
        LOG_FIRST_N(ERROR, 1) << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        LOG(ERROR) << ch;
    });
    google::SetStderrLogging(google::GLOG_INFO);

    FLAGS_logtostderr = false;  //设置日志消息是否转到标准输出而不是日志文件
    FLAGS_max_log_size = 100;     //设置最大日志文件大小（以MB为单位）
    //设置记录到标准输出的颜色消息（如果终端支持）
    FLAGS_colorlogtostderr = false;
    FLAGS_alsologtostderr = false;  //设置日志消息除了日志文件之外是否去标准输出
    FLAGS_log_prefix = true;        //设置日志前缀是否应该添加到每行输出
    FLAGS_log_dir = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_glog/logs";

    //thread_basic_logfile_example(); // 20~25ms
    test_backtrace();
    return 0;
}