#ifndef _BUILTINS_HPP_
#define _BUILTINS_HPP_

#include "builtintypesymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"
#include "typehelper.hpp"
#include "structsymbol.hpp"
#include "templatestructsymbol.hpp"

class TemplateStructDeclarationNode;

class BuiltIns
{
public:
    static Scope *global_scope;

    static BuiltInTypeSymbol *int_type;

    static StructSymbol *int_struct;

    static FunctionSymbol *int_assign;
    static FunctionSymbol *int_plus;
    static FunctionSymbol *int_minus;
    static FunctionSymbol *int_mul;
	static FunctionSymbol *int_eq;
	static FunctionSymbol *int_neq;
	static FunctionSymbol *int_div;
	static FunctionSymbol *int_mod;
    static FunctionSymbol *int_default_constructor;
	static FunctionSymbol *int_copy_constructor;

    static BuiltInTypeSymbol *void_type;

    static FunctionSymbol *putchar_func;
    static FunctionSymbol *getchar_func;

	static TemplateStructSymbol *array_struct; 
};

#endif
