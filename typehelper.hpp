#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "templateinfo.hpp"

class Type;
class Scope;
class StructSymbol;
class FunctionSymbol;
class OverloadedFunctionSymbol;

class TypeHelper
{
public:

    static FunctionSymbol* getConversion(Type *lhs, Type *rhs);

    static const Type* fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateInfo& template_info);
    
    static bool isConvertable(Type *lhs, Type *rhs);
    
	static FunctionSymbol* getCopyConstructor(Type *type);
	static FunctionSymbol* getDefaultConstructor(Type *type);

	static OverloadedFunctionSymbol* getOperatorCall(StructSymbol *sym);

	static Type* resolveType(std::string name, Scope *sc);
};

#endif
