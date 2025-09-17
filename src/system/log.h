#pragma once

// Set compile-time log-level. This determines which log statements are included in the compiled code.
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

#define GM_FILE_NAME __FILE_NAME__
#define GM_FUNCTION_NAME __func__
#define GM_LINE_NUMBER __LINE__

#define GM_TAG ::Game::tag(GM_FILE_NAME, GM_FUNCTION_NAME, GM_LINE_NUMBER)

#define GM_LOG_TRACE(message, ...) SPDLOG_TRACE(message, ##__VA_ARGS__)
#define GM_LOG_DEBUG(message, ...) SPDLOG_DEBUG(message, ##__VA_ARGS__)
#define GM_LOG_INFO(message, ...) SPDLOG_INFO(message, ##__VA_ARGS__)
#define GM_LOG_WARNING(message, ...) SPDLOG_WARN(message, ##__VA_ARGS__)
#define GM_LOG_ERROR(message, ...) SPDLOG_ERROR(message, ##__VA_ARGS__)
#define GM_LOG_CRITICAL(message, ...) SPDLOG_CRITICAL(message, ##__VA_ARGS__)

#define GM_LOG_T(message, ...) GM_LOG_TRACE(message, ##__VA_ARGS__)
#define GM_LOG_D(message, ...) GM_LOG_DEBUG(message, ##__VA_ARGS__)
#define GM_LOG_I(message, ...) GM_LOG_INFO(message, ##__VA_ARGS__)
#define GM_LOG_W(message, ...) GM_LOG_WARNING(message, ##__VA_ARGS__)
#define GM_LOG_E(message, ...) GM_LOG_ERROR(message, ##__VA_ARGS__)
#define GM_LOG_C(message, ...) GM_LOG_CRITICAL(message, ##__VA_ARGS__)

namespace Game {
    typedef spdlog::level::level_enum LogLevel;

    void initialize_log(LogLevel level);

    // Set runtime log-level. This determines which log statements are actually printed at runtime.
    // This does not matter if the compile-time log-level is higher and the log statements are compiled out.
    void set_log_level(LogLevel level);

    std::string tag(const char* file_name, const char* function_name, int line_number);
}
