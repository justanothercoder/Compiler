#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>
#include <boost/optional.hpp>
#include "typevisitor.hpp"

class Symbol;
class FunctionSymbol;

enum class TypeKind { BUILTIN, STRUCT, FUNCTION, OVERLOADEDFUNCTION, REFERENCE, POINTER, CONSTTYPE };

class Type
{
public:

	virtual ~Type();

	virtual void accept(TypeVisitor* visitor) const = 0;

    virtual std::string getName() const = 0;
    virtual size_t getSize() const = 0;
    
    virtual TypeKind getTypeKind() const = 0;

	virtual bool isConvertableTo(const Type *type) const = 0;
	virtual boost::optional<int> rankOfConversion(const Type *type) const = 0;

	virtual bool isReference() const;
	virtual bool isConst() const;

	virtual FunctionSymbol* getConversionTo(const Type *type) const = 0;

	virtual const Symbol* getSymbol() const;

	virtual const Type* getUnqualifiedType() const;
};

#endif
