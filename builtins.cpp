#include "builtins.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

BuiltInTypeSymbol *BuiltIns::int_type = new BuiltInTypeSymbol("int", GlobalConfig::int_size);
ReferenceType *BuiltIns::int_ref = TypeHelper::getReferenceType(int_type);

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=", FunctionTypeInfo(BuiltIns::int_ref, {BuiltIns::int_ref, BuiltIns::int_type}), BuiltIns::global_scope, true);
FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+", FunctionTypeInfo(BuiltIns::int_type, {BuiltIns::int_type, BuiltIns::int_type}), BuiltIns::global_scope, true);
FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-", FunctionTypeInfo(BuiltIns::int_type, {BuiltIns::int_type, BuiltIns::int_type}), BuiltIns::global_scope, true);
FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*", FunctionTypeInfo(BuiltIns::int_type, {BuiltIns::int_type, BuiltIns::int_type}), BuiltIns::global_scope, true);
