#include "consttype.hpp"

ConstType::ConstType(Type *const_type) : const_type(const_type) { }

Type* ConstType::getConstType() const { return const_type; } 

string ConstType::getName() const { return "const " + const_type->getName(); }

int ConstType::getSize() const { return const_type->getSize(); }
