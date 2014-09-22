#ifndef _BUILTINS_HPP_
#define _BUILTINS_HPP_

#include "builtintypesymbol.hpp"
#include "templatestructsymbol.hpp"

class AST;

class BuiltIns
{
public:

	static void defineBuiltIns();

    static Scope *global_scope;
	static Type *void_type;
	static Type *int_type;
};

#endif
