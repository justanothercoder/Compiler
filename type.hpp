#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>

using std::string;

enum class TypeKind { BUILTIN, STRUCT, FUNCTION, OVERLOADEDFUNCTION, REFERENCE };

class Type
{
public:

    virtual string getName() const = 0;
    virtual int getSize() const = 0;
    
    virtual TypeKind getTypeKind() const = 0;

    bool isReference() const;
};

#endif
