#ifndef LOGGER_H
#define LOGGER_H

#include <ostream>

// Configure Logger to be turned on/off
#define ENABLE_LOGGER true

#if ENABLE_LOGGER
    #define LOG(msg)            std::cout << msg
    #define LOG_WITH_ENDL(msg)  std::cout << msg << std::endl;
#else
    #define LOG(...)
    #define LOG_WITH_ENDL(msg)
#endif

#endif // LOGGER_H
