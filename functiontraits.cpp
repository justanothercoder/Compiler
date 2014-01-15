#include "functiontraits.hpp"

FunctionTraits::FunctionTraits(bool is_method, bool is_constructor, bool is_operator) : is_method(is_method), is_constructor(is_constructor), is_operator(is_operator) { }

FunctionTraits::FunctionTraits(std::initializer_list<bool> init_list)
{
    is_method      = *(std::begin(init_list) + 0);
    is_constructor = *(std::begin(init_list) + 1);
    is_operator    = *(std::begin(init_list) + 2);
}
