#pragma once

#if defined(_WIN32)
    #if defined(_WIN64)
        #define DD_PLATFORM_WINDOWS
    #else
        #error "Unsupported platform: Windows x86"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #define DD_PLATFORM_MACOS
#elif defined(__linux__) || defined(__unix__)
    #define DD_PLATFORM_LINUX
#else
    #error "Unknown platform"
#endif

#if defined(_MSC_VER)
    #define DD_COMPILER_MSVC
#elif defined(__clang__)
    #define DD_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define DD_COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#if defined(NDEBUG)
    #define DD_RELEASE
    #define DD_BUILD_RELEASE
#else
    #define DD_DEBUG
    #define DD_BUILD_DEBUG
#endif
