#pragma once

#include "environment.h"

#if defined(DD_PLATFORM_MACOS) || defined(DD_PLATFORM_LINUX)
    #define DD_PRINT_UNIX_STACKTRACE
    #elif defined(DD_PLATFORM_WINDOWS)
    #define DD_PRINT_WINDOWS_STACKTRACE
#endif

namespace dd {
    void initializeErrorSignalHandlers();

    void handleErrorSignal(int signal);

    void printStacktrace(int signal);

    std::string getSignalName(int signal);

    std::string getSignalDescription(int signal);

#ifdef DD_PRINT_UNIX_STACKTRACE
    void printUnixStacktrace();

    void demangleUnixStacktraceLine(std::string* stacktraceLine);
#endif
}
