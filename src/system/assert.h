#pragma once

#include "system/log.h"
#include "system/environment.h"

#ifdef GM_DEBUG
    #define GM_ENABLE_BREAK
    #define GM_ENABLE_ASSERT
#endif

#ifdef GM_ENABLE_BREAK
    #if defined(GM_COMPILER_MSVC)
        #define GM_BREAK() __debugbreak()
    #elif defined(GM_COMPILER_CLANG) || defined(GM_COMPILER_GCC)
        #define GM_BREAK() __builtin_trap()
    #else
        #error "GM_BREAK() not implemented for this compiler"
    #endif
#else
    #define GM_BREAK()
#endif

#ifdef GM_ENABLE_ASSERT
    namespace Game {
        void print_assert_error(std::string_view tag, std::string_view expression, std::string_view message);
    }

    #define GM_ASSERT(expression, message) \
            if (expression) {\
                /* Continue */\
            } else {\
                ::Game::print_assert_error(GM_TAG, #expression, message);\
                GM_BREAK(); \
            }
#else
    #define GM_ASSERT(expression)
#endif
