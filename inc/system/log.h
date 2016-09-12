#pragma once

#ifndef TESTING
    #include <nrf_log.h>

    #define SYSTEM_LOG_ERROR(...)                     NRF_LOG_ERROR(__VA_ARGS__)
    #define SYSTEM_LOG_WARNING(...)                   NRF_LOG_WARNING( __VA_ARGS__)
    #define SYSTEM_LOG_INFO(...)                      NRF_LOG_INFO( __VA_ARGS__)
    #define SYSTEM_LOG_DEBUG(...)                     NRF_LOG_DEBUG( __VA_ARGS__)
    #define SYSTEM_LOG_RAW_INFO(...)                  NRF_LOG_RAW_INFO( __VA_ARGS__)
#else
    #define SYSTEM_LOG_ERROR(...)                     // supress log output while testing
    #define SYSTEM_LOG_WARNING(...)
    #define SYSTEM_LOG_INFO(...)
    #define SYSTEM_LOG_DEBUG(...)
    #define SYSTEM_LOG_RAW_INFO(...)
#endif