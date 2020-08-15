#pragma once

// Logging strategy:
// - put everything in log file
// - stdout only above debug level

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define LOGGER_NAME         "main"
#define LOG_FILE            "cwotbyt.log"
#define LOG_PATTERN_REL     "[%d/%m/%Y %T.%e] %v"
#define LOG_PATTERN_DBG     "[%d/%m/%Y %T.%e] [%l] %v"

#define LOG_DEBUG(...)      spdlog::get(LOGGER_NAME)->debug(__VA_ARGS__)
#define LOG_INFO(...)       spdlog::get(LOGGER_NAME)->info(__VA_ARGS__)
#define LOG_ERROR(...)      spdlog::get(LOGGER_NAME)->error(__VA_ARGS__)

class Log
{
public:
  static inline void Init()
  {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
#ifdef _DEBUG
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern(LOG_PATTERN_DBG);
#else
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern(LOG_PATTERN_REL);
#endif

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(LOG_FILE, true);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern(LOG_PATTERN_DBG);

    std::vector<spdlog::sink_ptr> sinks({ console_sink, file_sink });
    auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, begin(sinks), end(sinks));
    logger->set_level(spdlog::level::debug);

    spdlog::register_logger(logger);

    LOG_DEBUG("Logger initialized");
  }

private:
  Log() {};
};