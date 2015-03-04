#ifndef _PARAMINFO_HPP_
#define _PARAMINFO_HPP_

#include <string>
#include "typeinfo.hpp"

class ParamInfo
{
public:

    ParamInfo(std::string name, TypeInfo type_info);

    const std::string& name() const;
    const TypeInfo& typeInfo() const;

    TypeInfo& typeInfo();

    std::string toString() const;

private:
    std::string name_;
    TypeInfo type_info;
};

#endif
