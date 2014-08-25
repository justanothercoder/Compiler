#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>
#include <boost/optional.hpp>

using std::string;

enum class TypeKind { BUILTIN, STRUCT, FUNCTION, OVERLOADEDFUNCTION, REFERENCE, POINTER, VARIABLETYPE };

class Type
{
public:

    virtual string getName() const = 0;
    virtual size_t getSize() const = 0;
    
    virtual TypeKind getTypeKind() const = 0;

	virtual bool isConvertableTo(const Type *type) const = 0;
	virtual boost::optional<int> rankOfConversion(const Type *type) const = 0;

	virtual bool isReference() const = 0;
	virtual bool isConst() const = 0;
};

#endif
