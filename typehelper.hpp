#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "typeinfo.hpp"
#include "referencetype.hpp"
#include "scope.hpp"

using std::map;

class TypeHelper
{
public:

    static Type* fromTypeInfo(TypeInfo type_info, Scope *scope);
    
    static bool isConvertable(Type *lhs, Type *rhs);
    
    static ReferenceType* getReferenceType(Type *target);

    static map<Type*, ReferenceType*> references;
};

#endif
