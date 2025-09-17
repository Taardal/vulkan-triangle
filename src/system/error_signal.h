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
    void initializeErrorSignalHandlers();

    void handleErrorSignal(int signal);

    void printStacktrace(int signal);

    std::string getSignalName(int signal);

    std::string getSignalDescription(int signal);

#ifdef GM_PRINT_UNIX_STACKTRACE
    void printUnixStacktrace();

    void demangleUnixStacktraceLine(std::string* stacktraceLine);
#endif
}
