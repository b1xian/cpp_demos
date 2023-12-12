#ifndef CPP11_DEMO_VISION_LOG_H
#define CPP11_DEMO_VISION_LOG_H


// ---------------------------spdlog---------------------------
#ifdef SPDLOG
#include "spdlog/cfg/env.h"
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/syslog_sink.h"
#include <spdlog/spdlog.h>
#include "spdlog/stopwatch.h"

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
#define VLOGT_LOGGER(logger_name,...) spdlog::get(logger_name)->trace(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGD_LOGGER(logger_name,...) spdlog::get(logger_name)->debug(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGI_LOGGER(logger_name,...) spdlog::get(logger_name)->info(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGW_LOGGER(logger_name,...) spdlog::get(logger_name)->warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGE_LOGGER(logger_name,...) spdlog::get(logger_name)->error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define VLOGC_LOGGER(logger_name,...) spdlog::get(logger_name)->critical(MAKE_SOURCE_LOC, __VA_ARGS__)
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




#endif //CPP11_DEMO_VISION_LOG_H
