#pragma once

#include "system/log.h"
#include "system/environment.h"

#ifdef DD_DEBUG
    #define DD_ENABLE_BREAK
    #define DD_ENABLE_ASSERT
#endif

#ifdef DD_ENABLE_BREAK
    #if defined(DD_COMPILER_MSVC)
    #define DD_BREAK() __debugbreak()
    #elif defined(DD_COMPILER_CLANG) || defined(DD_COMPILER_GCC)
    #define DD_BREAK() __builtin_trap()
    #else
        #error "DD_BREAK() not implemented for this compiler"
    #endif
#else
    #define DD_BREAK()
#endif

#ifdef DD_ENABLE_ASSERT
    #define DD_ASSERT(expression) \
            if (expression) {\
                /* Continue */\
            } else {\
                DD_LOG_CRITICAL("Assertion failed: {}", #expression); \
                DD_BREAK(); \
            }
#else
    #define DD_ASSERT(expression)
#endif
