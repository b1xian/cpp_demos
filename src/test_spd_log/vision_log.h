#ifndef TRUNK_VISION_PERCEPTION_LOG_H
#define TRUNK_VISION_PERCEPTION_LOG_H

#include <string>

#include "build_config.h"

// ---------------------------spdlog---------------------------
#ifdef SPDLOG
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

// 文件名和行号信息
#define MAKE_SOURCE_LOC spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}

// 输出到控制台
#define VLOGT(...) spdlog::trace(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGD(...) spdlog::debug(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGI(...) spdlog::info(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGW(...) spdlog::warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGE(...) spdlog::error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGC(...) spdlog::critical(MAKE_SOURCE_LOC, __VA_ARGS__)

// 创建logger
#define CREATE_BASIC_LOGGER_MT(...) spdlog::basic_logger_mt(__VA_ARGS__)
#define CREATE_BASIC_LOGGER_ST(...) spdlog::basic_logger_st(__VA_ARGS__)
#define CREATE_STDOUT_LOGGER_MT(...) spdlog::stdout_color_mt(__VA_ARGS__)
#define CREATE_STDOUT_LOGGER_ST(...) spdlog::stdout_color_st(__VA_ARGS__)
#define CREATE_STDERR_LOGGER_MT(...) spdlog::stderr_color_mt(__VA_ARGS__)
#define CREATE_STDERR_LOGGER_ST(...) spdlog::stderr_color_st(__VA_ARGS__)
#define CREATE_ROTATING_LOGGER_MT(...) spdlog::rotating_logger_mt(__VA_ARGS__)
#define CREATE_ROTATING_LOGGER_ST(...) spdlog::rotating_logger_st(__VA_ARGS__)
#define CREATE_DAILY_LOGGER_MT(...) spdlog::daily_logger_mt(__VA_ARGS__)
#define CREATE_DAILY_LOGGER_ST(...) spdlog::daily_logger_st(__VA_ARGS__)
#define CREATE_DAILY_LOGGER_FORMAT_MT(...) spdlog::daily_logger_format_mt(__VA_ARGS__)
#define CREATE_DAILY_LOGGER_FORMAT_ST(...) spdlog::daily_logger_format_st(__VA_ARGS__)
#define CREATE_SYSLOG_LOGGER_MT(...) spdlog::syslog_logger_mt(__VA_ARGS__)
#define CREATE_SYSLOG_LOGGER_ST(...) spdlog::syslog_logger_st(__VA_ARGS__)

// 使用指定的logger来输出日志
#ifdef LOG_TO_CONSOLE // 如果指定需要输出到控制台
#define VLOGT_LOGGER(logger_name,...) \
        //VLOGT(__VA_ARGS__);               \
        spdlog::get(logger_name)->trace(MAKE_SOURCE_LOC, __VA_ARGS__);
#define VLOGD_LOGGER(logger_name,...) \
        VLOGD(__VA_ARGS__);                 \
        spdlog::get(logger_name)->debug(MAKE_SOURCE_LOC, __VA_ARGS__);
#define VLOGI_LOGGER(logger_name,...) \
        VLOGI(__VA_ARGS__);                 \
        spdlog::get(logger_name)->info(MAKE_SOURCE_LOC, __VA_ARGS__);
#define VLOGW_LOGGER(logger_name,...) \
        VLOGW(__VA_ARGS__);                \
        spdlog::get(logger_name)->warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGE_LOGGER(logger_name,...) \
        VLOGE(__VA_ARGS__);                \
        spdlog::get(logger_name)->error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGC_LOGGER(logger_name,...) \
        VLOGC(__VA_ARGS__);                \
        spdlog::get(logger_name)->critical(MAKE_SOURCE_LOC, __VA_ARGS__)
#else
#define VLOGT_LOGGER(logger_name,...) spdlog::get(logger_name)->trace(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGD_LOGGER(logger_name,...) spdlog::get(logger_name)->debug(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGI_LOGGER(logger_name,...) spdlog::get(logger_name)->info(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGW_LOGGER(logger_name,...) spdlog::get(logger_name)->warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGE_LOGGER(logger_name,...) spdlog::get(logger_name)->error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGC_LOGGER(logger_name,...) spdlog::get(logger_name)->critical(MAKE_SOURCE_LOC, __VA_ARGS__)
#endif

#endif
// ---------------------------spdlog---------------------------



// ---------------------------glog---------------------------
#ifdef GLOG
#include <glog/logging.h>
#define VLOGI LOG(INFO)
#define VLOGW LOG(WARNING)
#define VLOGE LOG(ERROR)
#define VLOGC LOG(FATAL)
// ---------------------------glog---------------------------
#endif


namespace trunk {
namespace perception {

class Logger {
public:
#ifdef SPDLOG
    /**
     * @brief 设置全局的日志pattern和level。除此之外，可以为指定的logger设置对应的pattern和level。
     * @return
     */
    static int InitLogger() {
        /**
         * set GLOBAL log pattern:
         * %C-%m-%d %H:%M:%S.%f : [22-03-24 13:20:47.209429]
         * %^_%L_%$ : [_T_(trace), _D_(debug), _I_(INFO), _W_(WARN), _E_(ERROR), _C_(CRITICAL)]
         * %t : thread id
         * %n : logger name (if exists)
         * %@ : source file name and line number
         * %v : actual log content
         */
        spdlog::set_pattern("[%C-%m-%d %H:%M:%S.%f %^_%L_%$ %t] [%n %@] %v");
        // set GLOBAL log level
        spdlog::set_level(spdlog::level::LOG_LEVEL);
        return 0;
    }
//#define CREATE_BASIC_LOGGER_MT(...) spdlog::basic_logger_mt(__VA_ARGS__)
//#define CREATE_BASIC_LOGGER_ST(...) spdlog::basic_logger_st(__VA_ARGS__)
//#define CREATE_STDOUT_LOGGER_MT(...) spdlog::stdout_color_mt(__VA_ARGS__)
//#define CREATE_STDOUT_LOGGER_ST(...) spdlog::stdout_color_st(__VA_ARGS__)
//#define CREATE_STDERR_LOGGER_MT(...) spdlog::stderr_color_mt(__VA_ARGS__)
//#define CREATE_STDERR_LOGGER_ST(...) spdlog::stderr_color_st(__VA_ARGS__)
//#define CREATE_ROTATING_LOGGER_MT(...) spdlog::rotating_logger_mt(__VA_ARGS__)
//#define CREATE_ROTATING_LOGGER_ST(...) spdlog::rotating_logger_st(__VA_ARGS__)
//#define CREATE_DAILY_LOGGER_MT(...) spdlog::daily_logger_mt(__VA_ARGS__)
//#define CREATE_DAILY_LOGGER_ST(...) spdlog::daily_logger_st(__VA_ARGS__)
//#define CREATE_DAILY_LOGGER_FORMAT_MT(...) spdlog::daily_logger_format_mt(__VA_ARGS__)
//#define CREATE_DAILY_LOGGER_FORMAT_ST(...) spdlog::daily_logger_format_st(__VA_ARGS__)
//#define CREATE_SYSLOG_LOGGER_MT(...) spdlog::syslog_logger_mt(__VA_ARGS__)
//#define CREATE_SYSLOG_LOGGER_ST(...) spdlog::syslog_logger_st(__VA_ARGS__)

    // 线程id ： logger-name
    static void CreateBasicLoggerMt(const std::string& logger_name, const std::string& log_path, bool truncate=false) {
        spdlog::basic_logger_mt(logger_name, log_path, truncate);
    }
    static void CreateStdoutLoggerMt(const std::string& logger_name) {
        spdlog::stdout_color_mt(logger_name);
    }
#endif
};

// static init spdlog
#ifdef SPDLOG
#define INIT_VISION_LOG_RESULT_VAR(file, line) Init_result_##file_##line
#define INIT_VISION_LOG() \
         static int INIT_VISION_LOG_RESULT_VAR(__FILE__, __LINE__) = Logger::InitLogger();
INIT_VISION_LOG()
#undef INIT_VISION_LOG
#undef INIT_VISION_LOG_RESULT_VAR
#endif
}  // perception
}  // trunk
#endif //TRUNK_VISION_PERCEPTION_LOG_H
