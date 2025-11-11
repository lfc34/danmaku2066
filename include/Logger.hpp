/// WORKS ONLY WITH LOGGER OPTION TURNED ON
/// cmake -DENABLE_LOGGER=ON
#pragma once

// Latch (touch only if necessarry, otherwise use cmake directive)
// #define ENABLE_LOGGER 1

#include <string>
#include <iostream>

#ifdef ENABLE_LOGGER 

// LVL 0 - only impotant logs (critical errors (red))
// LVL 1 - important logs (all LVL 0 logs + warnings (yellow), game state (green))
// LVL 2 - misc (all LVL 0, LVL1 logs + misc messages (white))
namespace Logger {
    const std::string YELLOW = "\033[33m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";
    // Check if logger is enabled
    inline void status() {
        std::cout << GREEN << "Logger is enabled" << RESET << "\n";
    }
    // print red message, \n added automatically 
    inline void log_err(const std::string &err) {
        std::cerr << RED << err << RESET << "\n";
    }
    // print yellow message, \n added automatically
    inline void log_msg(const std::string &msg) {
        std::cout << YELLOW << msg << RESET << "\n";
    }
    // print green message, \n added automatically
    inline void log_grn(const std::string &msg) {
        std::cout << GREEN << msg << RESET << "\n";
    }
    // print white message, \n added automatically
    inline void log_clr(const std::string &msg) {
        std::cout << msg << "\n";
    }
};

#else 

namespace Logger {
    inline void status() {
        std::cout << "Logger is disabled\n";
    }
    inline void log_err(const std::string&){};
    inline void log_msg(const std::string&){};
    inline void log_grn(const std::string&){};
    inline void log_clr(const std::string&){};
}

#endif