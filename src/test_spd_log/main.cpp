//
// Created by sheldon on 2022/3/23.
//
#include <iostream>
#include <thread>
#include <vector>

#include "vision_log.h"

void basic_logfile_example() {
    try {
        auto logger = spdlog::basic_logger_mt("basic_logger", "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_spd_log/basic-log.txt");
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
    for(int i = 0 ; i < 10000; ++i) {
        spdlog::get("basic_logger")->info("spdlog::get(basic_logger) {}", i);

    }
    // todo 无法输出到日志文件
    std::vector<int> vec;
    vec.push_back(1);
    int v2 = vec.at(2);

}

void stdout_example() {
    // create color multi threaded logger
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("spdlog::get(console)");
    spdlog::get("stderr")->info("spdlog::get(stderr)");
}
void basic_example() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e %^__%L__%$] [%t] [%n %@] %v");
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    //spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Support for floats {0:.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("__{:<30}__", "left aligned");
    spdlog::info("__{:<30}__", "left");
    spdlog::info("__{:<30}__", "aligned");

    spdlog::set_level(spdlog::level::trace); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    //spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    //spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%n %f] [%^---%L---%$] [thread %t] %v");
    SPDLOG_TRACE("Some trace message with param {}", 42);

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    //SPDLOG_DEBUG("Some debug message");
    //SPDLOG_INFO("Some info message");
    //SPDLOG_WARN("Some warning message");
    //SPDLOG_ERROR("error message");
    //SPDLOG_CRITICAL("CRITICAL message");
    int a[4] = {1, 2, 3, 4};
    std::vector<int> v = {1, 2, 3};
    //fmt::print("{}\n", v);
}

void thread_log(const std::string logger_name) {
    //std::string log_path = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_spd_log/logs/" + logger_name + ".log";
    //SPDLOG_DEBUG("{0} path:{1}", logger_name, log_path);
    //try {
    //    //auto logger = spdlog::basic_logger_st(logger_name, log_path);
    //    auto logger1 = spdlog::basic_logger_mt(logger_name, log_path);
    //} catch (const spdlog::spdlog_ex &ex) {
    //    std::cout << "Log init failed: " << ex.what() << std::endl;
    //}
    for (int i = 0; i < 30000; ++i) {
        //VLOGT("thread {0}, value {1}", logger_name, i);
        spdlog::get("multithread")->trace("thread {0}, value {1}", logger_name, i);
        //spdlog::get(logger_name)->info(logger_name + "~~~~~~~~~~~~~~~~~hahah.~~~~~{0}", i);
    }
}

void thread_to1_basic_logfile_example() {
    std::string log_path = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_spd_log/logs/multithread.log";
    //auto logger = spdlog::basic_logger_mt("multithread", log_path, true);
    //auto logger = spdlog::basic_logger_st("multithread", log_path, true);
    spdlog::rotating_logger_mt("multithread", log_path, 1024 * 1024 * 5, 3);

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

void thread_log_to_n(const std::string logger_name) {
    std::string log_path = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_spd_log/logs/" + logger_name + ".log";
    //SPDLOG_DEBUG("{0} path:{1}", logger_name, log_path);
    try {
        //auto logger = spdlog::basic_logger_st(logger_name, log_path);
        auto logger1 = spdlog::basic_logger_mt(logger_name, log_path, true);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
    for (int i = 0; i < 30000; ++i) {
        //VLOGT("thread {0}, value {1}", logger_name, i);
        spdlog::get(logger_name)->trace("thread {0}, value {1}", logger_name, i);
        //spdlog::get(logger_name)->info(logger_name + "~~~~~~~~~~~~~~~~~hahah.~~~~~{0}", i);
    }
}
void thread_to_n_basic_logfile_example() {
    std::thread camera1_thread(thread_log_to_n, "camera1");
    std::thread camera2_thread(thread_log_to_n, "camera2");
    std::thread camera3_thread(thread_log_to_n, "camera3");
    std::thread camera4_thread(thread_log_to_n, "camera4");
    std::thread camera5_thread(thread_log_to_n, "camera5");
    std::thread camera6_thread(thread_log_to_n, "camera6");
    std::thread camera7_thread(thread_log_to_n, "camera7");
    std::thread camera8_thread(thread_log_to_n, "camera8");
    std::thread camera9_thread(thread_log_to_n, "camera9");
    std::thread camera10_thread(thread_log_to_n, "camera10");
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
void test_log_with_fileinfo() {
    //VLOGT("log with file info, value {}", 11);
    //VLOGD("log with file info, value {}", 11);
    //VLOGI("log with file info, value {}", 11);
    //VLOGW("log with file info, value {}", 11);
    //VLOGE("log with file info, value {}", 11);
    //VLOGC("log with file info, value {}", 11);

    std::string logger_name = "fileinfo";
    std::string log_path = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_spd_log/logs/fileinfo.log";
    CREATE_BASIC_LOGGER_MT(logger_name, log_path);

    for (int i = 0; i < 100; ++i) {
        VLOGT_LOGGER(logger_name, logger_name + "~~~{0}", i);
        VLOGD_LOGGER(logger_name, logger_name + "~~~{0}", i);
        VLOGI_LOGGER(logger_name, logger_name + "~~~{0}", i);
        VLOGW_LOGGER(logger_name, logger_name + "~~~{0}", i);
        VLOGE_LOGGER(logger_name, logger_name + "~~~{0}", i);
        VLOGC_LOGGER(logger_name, logger_name + "~~~{0}", i);
        //spdlog::get(logger_name)->info(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},
        //                               logger_name + "~~~~~~~~~~~~~~~~~fileinfo.~~~~~{0}", i);
        //spdlog::get(logger_name)->info(logger_name + "~~~~~~~~~~~~~~~~~fileinfo.~~~~~{0}", i);
    }
}

void test_backtrace() {
    // Debug messages can be stored in a ring buffer instead of being logged immediately.
// This is useful in order to display debug logs only when really needed (e.g. when error happens).
// When needed, call dump_backtrace() to see them.

    //spdlog::enable_backtrace(32); // Store the latest 32 messages in a buffer. Older messages will be dropped.
    //for(int i = 0; i < 100; i++)
    //{
    //    spdlog::debug("Backtrace message {}", i); // not logged yet..
    //}
    //spdlog::dump_backtrace(); // log them now! show the last 32 messages



}

int main () {
    trunk::perception::Logger::InitLogger();
    //spdlog::set_pattern("[%C-%m-%d %H:%M:%S.%f %^_%L_%$ %t] [%n %@] %v");
    //spdlog::set_level(spdlog::level::trace); // Set global log level to debug
    //basic_example();
    //stdout_example();
    //basic_logfile_example();
    //thread_to1_basic_logfile_example();// 7ms
    //thread_to_n_basic_logfile_example();
    //
    //test_log_with_fileinfo();


    //VLOGI("trying match pair lane, prefit bev a0:{}, search pair position:{}", tracker.GetPreFittingLaneRes3D().a0, lane_position)
    std::string f1 = fmt::format("trying match pair lane, prefit bev a0:{0:.2f}, search pair position:{1}", 1.2456, 1);
    VLOGI(f1);
    std::string f2 = fmt::v8::format("trying match pair lane, prefit bev a0:{0:.2f}, search pair position:{1:d}", 1.2456, 1);
    VLOGI(f2);

    //test_backtrace();
    return 0;
}