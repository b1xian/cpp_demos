#ifndef CPP11_DEMO_SPDLOG_H
#define CPP11_DEMO_SPDLOG_H

#include "spdlog/cfg/env.h"
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/syslog_sink.h"
#include <spdlog/spdlog.h>
#include "spdlog/stopwatch.h"


#define MAKE_SOURCE_LOC spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}

#define REGISTER_BASIC_LOGGER_MT(...) spdlog::basic_logger_mt(__VA_ARGS__)
#define REGISTER_BASIC_LOGGER_ST(...) spdlog::basic_logger_st(__VA_ARGS__)
#define REGISTER_STDOUT_LOGGER_MT(...) spdlog::stdout_color_mt(__VA_ARGS__)
#define REGISTER_STDOUT_LOGGER_ST(...) spdlog::stdout_color_st(__VA_ARGS__)
#define REGISTER_STDERR_LOGGER_MT(...) spdlog::stderr_color_mt(__VA_ARGS__)
#define REGISTER_STDERR_LOGGER_ST(...) spdlog::stderr_color_st(__VA_ARGS__)
#define REGISTER_ROTATING_LOGGER_MT(...) spdlog::rotating_logger_mt(__VA_ARGS__)
#define REGISTER_ROTATING_LOGGER_ST(...) spdlog::rotating_logger_st(__VA_ARGS__)
#define REGISTER_DAILY_LOGGER_MT(...) spdlog::daily_logger_mt(__VA_ARGS__)
#define REGISTER_DAILY_LOGGER_ST(...) spdlog::daily_logger_st(__VA_ARGS__)
#define REGISTER_DAILY_LOGGER_FORMAT_MT(...) spdlog::daily_logger_format_mt(__VA_ARGS__)
#define REGISTER_DAILY_LOGGER_FORMAT_ST(...) spdlog::daily_logger_format_st(__VA_ARGS__)
#define REGISTER_SYSLOG_LOGGER_MT(...) spdlog::syslog_logger_mt(__VA_ARGS__)
#define REGISTER_SYSLOG_LOGGER_ST(...) spdlog::syslog_logger_st(__VA_ARGS__)

// 可以作为控制台打印，因为不需要对不同的线程/模块分离日志信息了，并设置开启项

#define SLOG_TRACE_IMPL(...) spdlog::trace(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_DEBUG_IMPL(...) spdlog::debug(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_INFO_IMPL(...) spdlog::info(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_WARN_IMPL(...) spdlog::warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_ERROR_IMPL(...) spdlog::error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_CRITICAL_IMPL(...) spdlog::critical(MAKE_SOURCE_LOC, __VA_ARGS__)

#define SLOG_TRACE(...) SLOG_TRACE_IMPL(__VA_ARGS__)
#define SLOG_DEBUG(...) SLOG_DEBUG_IMPL(__VA_ARGS__)
#define SLOG_INFO(...) SLOG_INFO_IMPL(__VA_ARGS__)
#define SLOG_WARN(...) SLOG_WARN_IMPL(__VA_ARGS__)
#define SLOG_ERROR(...) SLOG_ERROR_IMPL(__VA_ARGS__)
#define SLOG_CRITICAL(...) SLOG_CRITICAL_IMPL(__VA_ARGS__)



#define SLOG_TRACE_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->trace(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_DEBUG_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->debug(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_INFO_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->info(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_WARN_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->warn(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_ERROR_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->error(MAKE_SOURCE_LOC, __VA_ARGS__)
#define SLOG_CRITICAL_LOGGER_IMPL(logger_name, ...) spdlog::get(logger_name)->critical(MAKE_SOURCE_LOC, __VA_ARGS__)

#define SLOG_TRACE_LOGGER(...) SLOG_TRACE_LOGGER_IMPL(__VA_ARGS__)
#define SLOG_DEBUG_LOGGER(...) SLOG_DEBUG_LOGGER_IMPL(__VA_ARGS__)
#define SLOG_INFO_LOGGER(...) SLOG_INFO_LOGGER_IMPL(__VA_ARGS__)
#define SLOG_WARN_LOGGER(...) SLOG_WARN_LOGGER_IMPL(__VA_ARGS__)
#define SLOG_ERROR_LOGGER(...) SLOG_ERROR_LOGGER_IMPL(__VA_ARGS__)
#define SLOG_CRITICAL_LOGGER(...) SLOG_CRITICAL_LOGGER_IMPL(__VA_ARGS__)


class SpdLog {

};


#endif //CPP11_DEMO_SPDLOG_H
