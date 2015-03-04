#ifndef _VARINFO_HPP_
#define _VARINFO_HPP_

#include <string>
#include "variabletype.hpp"

class VarInfo
{
public:

    VarInfo(std::string name, VariableType type);

    const std::string& name() const;
    VariableType type() const;

    std::string toString() const;

private:

    std::string name_;
    VariableType type_;
};

#endif
