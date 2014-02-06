#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "referencetype.hpp"
#include "scope.hpp"
#include "structsymbol.hpp"
#include "functionhelper.hpp"
#include "templatestructsymbol.hpp"
#include "templatehelper.hpp"

using std::map;

class TypeHelper
{
public:

    static FunctionSymbol* getConversion(Type *lhs, Type *rhs);

    static bool existsConversion(Type *lhs, Type *rhs);

    static Type* fromTypeInfo(TypeInfo type_info, Scope *scope);
    
    static bool isConvertable(Type *lhs, Type *rhs);
    
    static ReferenceType* getReferenceType(Type *target);

    static map<Type*, ReferenceType*> references;
};

#endif
