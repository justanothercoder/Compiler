#ifndef _VARIABLETYPE_HPP_
#define _VARIABLETYPE_HPP_

#include <string>

class Type;

class VariableType
{
public:

    VariableType(const Type* type);
    VariableType(const Type* type, bool is_const);

    std::string getName() const;

    const Type* base() const; 
    bool isConst() const;
    bool isReference() const;
    int sizeOf() const;

    const Type* unqualified() const;

    bool operator==(const VariableType& vt) const;
    bool operator!=(const VariableType& vt) const;

private:

    const Type* type;
    bool is_const;
};

#endif
