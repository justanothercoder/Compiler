#include "functionaltype.hpp"

size_t FunctionalType::sizeOf() const { return 0; }
TypeKind FunctionalType::getTypeKind() const { return TypeKind::OVERLOADEDFUNCTION; }

bool FunctionalType::isConvertableTo(const Type *) const { return false; }
boost::optional<int> FunctionalType::rankOfConversion(const Type *) const { return boost::none; }
FunctionSymbol* FunctionalType::getConversionTo(const Type *) const { return nullptr; }
