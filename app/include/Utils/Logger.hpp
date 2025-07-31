//
// Created by Arjun Varma on 18/07/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

// #define ENABLE_LOGGING

#ifdef ENABLE_LOGGING
#include <iostream>
#include <mutex>
void safeLog(const std::string& message);
#else
inline void safeLog(const std::string&) {}
#endif

#endif // LOGGER_HPP

