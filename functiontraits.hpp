#ifndef _FUNCTIONTRAITS_HPP_
#define _FUNCTIONTRAITS_HPP_

#include <initializer_list>

struct FunctionTraits
{
    FunctionTraits(bool is_method, bool is_constructor, bool is_operator);
    FunctionTraits(std::initializer_list<bool> init_list);

    bool is_method;
    bool is_constructor;
    bool is_operator;
};

#endif
