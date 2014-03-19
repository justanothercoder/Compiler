#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "templateinfo.hpp"
#include "callhelper.hpp"

using std::map;

class TypeHelper
{
public:

    static FunctionSymbol* getConversion(VariableType lhs, VariableType rhs);

    static VariableType fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateInfo& template_info);
    
    static bool isConvertable(VariableType lhs, VariableType rhs);
    
	static FunctionSymbol* getCopyConstructor(VariableType type);

	static OverloadedFunctionSymbol* getOperatorCall(StructSymbol *sym);

	static Type* resolveType(string name, Scope *sc);
};

#endif
