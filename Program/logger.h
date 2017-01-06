#ifndef LOGGER_H
#define LOGGER_H

#include <ostream>

// Configure Logger to be turned on/off
#define ENABLE_LOGGER true

#if ENABLE_LOGGER
    #define LOG(msg)    std::cout << msg
#else
    #define LOG(...)
#endif

#endif // LOGGER_H
