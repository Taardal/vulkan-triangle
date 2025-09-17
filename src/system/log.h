#pragma once

#include "spdlog/common.h"
#include <spdlog/spdlog.h>

#ifndef __FILE_NAME__
    // #define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#define DD_FILENAME __FILE_NAME__
#define DD_FUNCTION_NAME __func__
#define DD_LINE_NUMBER __LINE__

#define DD_TAG() ::dd::tag(DD_FILENAME, DD_FUNCTION_NAME, DD_LINE_NUMBER)
#define DD_TAG_MESSAGE(message) ::dd::tag_message(DD_FILENAME, DD_FUNCTION_NAME, DD_LINE_NUMBER, message)

#define log_i(message, ...) ::spdlog::info("{}", DD_TAG_MESSAGE(message), ##__VA_ARGS__)
// #define log_i(message, ...) ::spdlog::info(message, ##__VA_ARGS__)

namespace dd {
    namespace log = spdlog;

    typedef spdlog::level::level_enum log_level;

    std::string tag(const char* filename, const char* functionName, uint32_t lineNumber);

    std::string tag_message(const char* filename, const char* functionName, uint32_t lineNumber, const char* message);
}

#define DD_LOG_TRACE(message, ...)  std::cout << message << std::endl;
#define DD_LOG_DEBUG(message, ...) std::cout << message << std::endl;
#define DD_LOG_INFO(message, ...) std::cout << message << std::endl;
#define DD_LOG_WARN(message, ...) std::cout << message << std::endl;
#define DD_LOG_ERROR(message, ...) std::cerr << message << std::endl;
#define DD_LOG_CRITICAL(message, ...) std::err << message << std::endl;

#define BL_LOG_TRACE(message, ...)  std::cout << message << std::endl;
#define BL_LOG_DEBUG(message, ...) std::cout << message << std::endl;
#define BL_LOG_INFO(message, ...) std::cout << message << std::endl;
#define BL_LOG_WARN(message, ...) std::cout << message << std::endl;
#define BL_LOG_ERROR(message, ...) std::cerr << message << std::endl;
#define BL_LOG_CRITICAL(message, ...) std::err << message << std::endl;

// namespace dd {
//     enum class LogLevel {
//         None = 0,
//         Trace,
//         Debug,
//         Info,
//         Warn,
//         Error,
//         Critical
//     };

//     class Log {
//     public:
//         static void initialize(LogLevel level);

//         static std::string tag(const char* filename, const char* functionName, uint32_t lineNumber);

//         static std::string tagMessage(const char* filename, const char* functionName, uint32_t lineNumber, std::string_view message);

//         template<typename... T>
//         static void trace(std::string_view message, const T&... args) {
//             spdlog::trace(message, args...);
//         }

//         template<typename... T>
//         static void debug(std::string_view message, const T&... args) {
//             spdlog::debug(message, args...);
//         }

//         template<typename... T>
//         static void info(std::string_view message, const T&... args) {
//             spdlog::info(message, args...);
//         }

//         template<typename... T>
//         static void warn(std::string_view message, const T&... args) {
//             spdlog::warn(message, args...);
//         }

//         template<typename... T>
//         static void error(std::string_view message, const T&... args) {
//             spdlog::error(message, args...);
//         }

//         template<typename... T>
//         static void critical(std::string_view message, const T&... args) {
//             spdlog::critical(message, args...);
//         }

//         template<typename... T>
//         static void log(LogLevel logLevel, std::string_view message, const T&... args) {
//             if (logLevel == LogLevel::Critical) {
//                 critical(message, args...);
//             } else if (logLevel == LogLevel::Error) {
//                 error(message, args...);
//             } else if (logLevel == LogLevel::Warn) {
//                 warn(message, args...);
//             } else if (logLevel == LogLevel::Info) {
//                 info(message, args...);
//             } else if (logLevel == LogLevel::Debug) {
//                 debug(message, args...);
//             } else if (logLevel == LogLevel::Trace) {
//                 trace(message, args...);
//             }
//         }

//     private:
//         static spdlog::level::level_enum getSpdLogLevel(LogLevel level);
//     };
// }
