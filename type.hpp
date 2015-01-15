#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>
#include <boost/optional.hpp>
#include <memory>

class Symbol;
class FunctionSymbol;

enum class TypeKind { BUILTIN, STRUCT, FUNCTION, OVERLOADEDFUNCTION, REFERENCE, POINTER, ARRAY, TEMPLATEFUNCTION };

class Type
{
public:

    virtual ~Type();

    virtual std::string getName() const = 0;
    virtual size_t sizeOf() const = 0;

    virtual TypeKind getTypeKind() const = 0;

    virtual bool isConvertableTo(const Type *type) const = 0;
    virtual boost::optional<int> rankOfConversion(const Type *type) const = 0;

    virtual bool isReference() const;
    virtual bool isObjectType() const;

    virtual const FunctionSymbol* getConversionTo(const Type *type) const = 0;
    virtual const Type* removeRef() const;
};

#endif
