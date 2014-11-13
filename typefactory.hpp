#ifndef _TYPEFACTORY_HPP_
#define _TYPEFACTORY_HPP_

#include <map>
#include "functiontypeinfo.hpp"

class FunctionType;

class TypeFactory
{
public:

    static const Type* getPointer(const Type *type);
    static const Type* getReference(const Type *type);
    static const Type* getConst(const Type *type);
    static const Type* getArray(const Type *type, int size);

    static const FunctionType* getFunctionType(const Type *return_type, const FunctionTypeInfo& func_info);

private:
    TypeFactory();
};

#endif
