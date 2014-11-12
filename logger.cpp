#include "logger.hpp"

void Logger::log(std::string s)
{
    std::cerr << "Log: " << s << '\n';
}
