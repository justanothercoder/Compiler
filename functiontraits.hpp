#ifndef _FUNCTIONTRAITS_HPP_
#define _FUNCTIONTRAITS_HPP_

#include <initializer_list>

struct FunctionTraits
{
    FunctionTraits(bool is_method, bool is_constructor, bool is_operator);
    FunctionTraits(std::initializer_list<bool> init_list);

    static FunctionTraits simple();
    static FunctionTraits oper();
    static FunctionTraits method();
    static FunctionTraits methodOper();
    static FunctionTraits constructor();

    bool is_method;
    bool is_constructor;
    bool is_operator;
};

#endif
