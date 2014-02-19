#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "referencetype.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "templatestructsymbol.hpp"

using std::map;

class TypeHelper
{
public:

    static FunctionSymbol* getConversion(Type *lhs, Type *rhs);

    static bool existsConversion(Type *lhs, Type *rhs);

    static Type* fromTypeInfo(TypeInfo type_info, Scope *scope, const TemplateStructSymbol *template_sym = nullptr, vector<ExprNode*> expr = { });
    
    static bool isConvertable(Type *lhs, Type *rhs);
    
	static FunctionSymbol* getCopyConstructor(Type *type);
    static ReferenceType* getReferenceType(Type *target);

    static map<Type*, ReferenceType*> references;
};

#endif
