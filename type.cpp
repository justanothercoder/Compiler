#include "type.hpp"

Type::~Type() { } 

const Type* Type::removeRef() const { return this; } 
bool Type::isArray() const { return false; }
bool Type::isPointer() const { return false; }
bool Type::isReference() const { return false; }
bool Type::isObjectType() const { return false; }
