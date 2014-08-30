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

    static FunctionSymbol* getConversion(Type *lhs, Type *rhs);

    static const Type* fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateInfo& template_info);
    
    static bool isConvertable(Type *lhs, Type *rhs);
    
	static FunctionSymbol* getCopyConstructor(Type *type);
	static FunctionSymbol* getDefaultConstructor(Type *type);

	static OverloadedFunctionSymbol* getOperatorCall(StructSymbol *sym);

	static Type* resolveType(string name, Scope *sc);
};

#endif
