#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <string>
#include <memory>
#include <boost/optional.hpp>

class Symbol;
class FunctionalSymbol;

class Type
{
public:

    virtual ~Type();

    virtual std::string typeName() const = 0;
    virtual size_t sizeOf() const = 0;

    virtual bool isConvertableTo(const Type *type) const = 0;
    virtual boost::optional<int> rankOfConversion(const Type *type) const = 0;

    virtual bool isArray() const;
    virtual bool isPointer() const;
    virtual bool isReference() const;
    virtual bool isObjectType() const;

    virtual const FunctionalSymbol* getConversionTo(const Type *type) const = 0;
    virtual const Type* removeRef() const;
};

#endif
