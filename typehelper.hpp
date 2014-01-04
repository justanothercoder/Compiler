#ifndef _TYPEHELPER_HPP_
#define _TYPEHELPER_HPP_

#include <map>

#include "referencetype.hpp"

using std::map;

class TypeHelper
{
public:

    static bool isConvertable(Type *lhs, Type *rhs);
    
    static ReferenceType* getReferenceType(Type *target);

    static map<Type*, ReferenceType*> references;
};

#endif
