#ifndef _GLOBALCONFIG_HPP_
#define _GLOBALCONFIG_HPP_

#include <string>

class GlobalConfig
{
public:
    static const int int_size = sizeof(int*);

    static std::string getCodeOperatorName(std::string op);
};

#endif
