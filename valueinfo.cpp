#include "valueinfo.hpp"

ValueInfo::ValueInfo(VariableType type, bool is_left_value) : type_(type), is_left_value(is_left_value)
{

}

VariableType ValueInfo::type() const { return type_; }
bool ValueInfo::isLeftValue() const { return is_left_value; }
