#ifndef _GLOBALCONFIG_HPP_
#define _GLOBALCONFIG_HPP_

#include <string>
#include <map>
#include <boost/optional.hpp>

class GlobalConfig
{
public:

    GlobalConfig();

    const int int_size;

    std::map<std::string, bool> flags;
    
    boost::optional<bool> flagValue(std::string flag) const;
    void setFlag(std::string flag, bool value = true);

    std::string getCodeOperatorName(std::string op) const;
};

#endif
