//
// Created by Arjun Varma on 18/07/25.
//

#include "../../include/Utils/Logger.hpp"

#ifdef ENABLE_LOGGING
void safeLog(const std::string& message) {
    static std::mutex log_mutex;
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << message << std::endl;
}
#endif
