#include "type.hpp"

bool Type::isReference() const { return getTypeKind() == TypeKind::REFERENCE; }
