#pragma once

// Logging strategy:
// - put everything in log file
// - stdout only above debug level

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define LOGGER_NAME         "main"
#define LOG_FILE            "cwotbyt.log"
#define LOG_PATTERN         "[%d/%m/%Y %T.%e] [%l] %v"

#define LOG_DEBUG(...)      spdlog::get(LOGGER_NAME)->debug(__VA_ARGS__)
#define LOG_INFO(...)       spdlog::get(LOGGER_NAME)->info(__VA_ARGS__)
#define LOG_ERROR(...)      spdlog::get(LOGGER_NAME)->error(__VA_ARGS__)

class Log
{
public:
  static inline void Init()
  {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern(LOG_PATTERN);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(LOG_FILE, true);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern(LOG_PATTERN);

    std::vector<spdlog::sink_ptr> sinks({ console_sink, file_sink });
    auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, begin(sinks), end(sinks));
    spdlog::register_logger(logger);

    LOG_DEBUG("Logger initialized");
  }
};