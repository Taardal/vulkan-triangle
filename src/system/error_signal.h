#pragma once

#include "environment.h"

#if defined(GM_PLATFORM_MACOS) || defined(GM_PLATFORM_LINUX)
    #define GM_PRINT_UNIX_STACKTRACE
    #include <cxxabi.h>
    #include <execinfo.h>
#elif defined(GM_PLATFORM_WINDOWS)
    #define GM_PRINT_WINDOWS_STACKTRACE
#endif

namespace Game {
    void initialize_error_signal_handlers();

    void handle_error_signal(int signal);

    void print_stacktrace(int signal);

    std::string get_signal_name(int signal);

    std::string get_signal_description(int signal);

#ifdef GM_PRINT_UNIX_STACKTRACE
    void print_unix_stacktrace();

    void demangle_unix_stacktrace_line(std::string* stacktraceLine);
#endif
}
