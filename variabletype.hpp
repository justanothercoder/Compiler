#ifndef _VARIABLETYPE_HPP_
#define _VARIABLETYPE_HPP_

#include <string>

#include "type.hpp"
#include "globalconfig.hpp"

using std::string;

class VariableType
{
public:
	VariableType();
	VariableType(Type *type, bool is_ref = false, bool is_const = false);

	VariableType& operator =(const VariableType& vt) = default;
	VariableType(const VariableType&);
	VariableType(VariableType&& vt);

	string getName() const;
	size_t getSize() const;

	string getTypeName() const;

	bool operator==(VariableType vt) const;
	bool operator!=(VariableType vt) const;

	int rankOfConversion(VariableType vt);

public:
	Type *type;
	bool is_ref, is_const;	
};

#endif

