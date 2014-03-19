#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "referencetype.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "templatestructsymbol.hpp"
#include "callhelper.hpp"

using std::map;

class TypeHelper
{
public:

    static FunctionSymbol* getConversion(VariableType lhs, VariableType rhs);

    static VariableType fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateStructSymbol *template_sym = nullptr, vector<ExprNode*> expr = { });
    
    static bool isConvertable(VariableType lhs, VariableType rhs);
    
	static FunctionSymbol* getCopyConstructor(VariableType type);

	static OverloadedFunctionSymbol* getOperatorCall(StructSymbol *sym);

	static Type* resolveType(string name, Scope *sc);
};

#endif
