#ifndef _FUNCTIONHELPER_HPP_
#define _FUNCTIONHELPER_HPP_

#include <set>
#include <iostream>

#include "typehelper.hpp"
#include "functiontypeinfo.hpp"

#include "codeobject.hpp"

using std::set;

class FunctionHelper
{
public:

	static FunctionSymbol* makeDefaultCopyConstructor(StructSymbol *struc);
	static FunctionSymbol* makeDefaultConstructor(StructSymbol *struc);
};   

#endif
