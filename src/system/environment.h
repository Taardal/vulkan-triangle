#pragma once

#if defined(_WIN32)
    #if defined(_WIN64)
        #define GM_PLATFORM_WINDOWS
    #else
        #error "Unsupported platform: Windows x86"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #define GM_PLATFORM_MACOS
#elif defined(__linux__) || defined(__unix__)
    #define GM_PLATFORM_LINUX
#else
    #error "Unknown platform"
#endif

#if defined(_MSC_VER)
    #define GM_COMPILER_MSVC
#elif defined(__clang__)
    #define GM_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define GM_COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#if defined(NDEBUG)
    #define GM_RELEASE
    #define GM_BUILD_RELEASE
#else
    #define GM_DEBUG
    #define GM_BUILD_DEBUG
#endif
