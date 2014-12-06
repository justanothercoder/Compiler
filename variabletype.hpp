#ifndef _VARIABLETYPE_HPP_
#define _VARIABLETYPE_HPP_

class Type;

class VariableType
{
public:

    VariableType(const Type* type, bool is_const);

    std::string getName() const;

    const Type* base() const; 
    bool isConst() const;
    bool isReference() const;
    int sizeOf() const;

    const Type* unqualified() const;

private:

    const Type* type;
    bool is_const;
};

#endif
