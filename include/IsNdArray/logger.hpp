#pragma once

#include <IsNdArray/IsNdArray.hpp>

#include <cstdio>

#define _NBLA_LOG_STDOUT(...)                               \
    {                                                       \
        std::printf("%s:%d :", __FILE__, __LINE__);         \
        std::printf(__VA_ARGS__);                           \
        std::fflush(stdout);                                \
    }

#define _NBLA_LOG_NONE(...)                                 \
    {}

#define NBLA_LOG_TRACE _NBLA_LOG_NONE
#define NBLA_LOG_DEBUG _NBLA_LOG_NONE
#define NBLA_LOG_INFO _NBLA_LOG_NONE
#define NBLA_LOG_WARN _NBLA_LOG_NONE
#define NBLA_LOG_ERROR _NBLA_LOG_STDOUT
#define NBLA_LOG_CRITICAL _NBLA_LOG_STDOUT
