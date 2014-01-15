#ifndef _BUILTINS_HPP_
#define _BUILTINS_HPP_

#include "builtintypesymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"
#include "typehelper.hpp"

class BuiltIns
{
public:
    static Scope *global_scope;

    static BuiltInTypeSymbol *int_type;
    static ReferenceType *int_ref;

    static FunctionSymbol *int_assign;
    static FunctionSymbol *int_plus;
    static FunctionSymbol *int_minus;
    static FunctionSymbol *int_mul;
};

#endif
