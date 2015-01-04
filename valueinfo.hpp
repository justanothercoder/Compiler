#ifndef _VALUEINFO_HPP_
#define _VALUEINFO_HPP_

#include "variabletype.hpp"

class ValueInfo
{
public:

    ValueInfo(VariableType type, bool is_left_value);

    VariableType type() const;
    bool isLeftValue() const;

private:
    VariableType type_;
    bool is_left_value;    
};

#endif
