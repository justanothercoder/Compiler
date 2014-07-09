#include "variabletype.hpp"

VariableType::VariableType() : VariableType(nullptr) { }
VariableType::VariableType(Type *type, bool is_ref, bool is_const) : type(type), is_ref(is_ref), is_const(is_const) { }
	
VariableType::VariableType(const VariableType& vt) : type(vt.type), is_ref(vt.is_ref), is_const(vt.is_const) { }
VariableType::VariableType(VariableType&& vt) : type(vt.type), is_ref(vt.is_ref), is_const(vt.is_const) { vt.type = nullptr; }
		
string VariableType::getName() const
{
	string res = "";

	if ( is_const )
		res += "const~";

	res += type->getName();

	if ( is_ref )
		res += "~ref";

	return res;
}

size_t VariableType::getSize() const { return is_ref ? GlobalConfig::int_size : type->getSize(); }
	
string VariableType::getTypeName() const { return type->getName(); }
	
bool VariableType::operator==(VariableType vt) const { return type == vt.type && is_const == vt.is_const && is_ref == vt.is_ref; }
bool VariableType::operator!=(VariableType vt) const { return !(*this == vt); }
