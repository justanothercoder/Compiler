#ifndef _VARIABLETYPE_HPP_
#define _VARIABLETYPE_HPP_

#include <string>

#include "type.hpp"
#include "globalconfig.hpp"

class VariableType : public Type
{
public:
	VariableType();
	VariableType(const Type *type, bool is_ref = false, bool is_const = false);

	VariableType& operator =(const VariableType& vt) = default;
	VariableType(const VariableType&);
	VariableType(VariableType&& vt);

	std::string getName() const;
	size_t getSize() const;

	std::string getTypeName() const;

	bool operator==(const VariableType& vt) const;
	bool operator!=(const VariableType& vt) const;

    TypeKind getTypeKind() const override;
	bool isConvertableTo(const Type *type) const override;
	boost::optional<int> rankOfConversion(const Type *t) const override;

	bool isReference() const override;
	bool isConst() const override;

public:
	const Type *type;
	bool is_ref, is_const;	
};

#endif

