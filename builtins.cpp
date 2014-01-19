#include "builtins.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

BuiltInTypeSymbol *BuiltIns::int_type = new BuiltInTypeSymbol("~~int", GlobalConfig::int_size);

StructSymbol *BuiltIns::int_struct = new StructSymbol("int", BuiltIns::global_scope);
ReferenceType *BuiltIns::int_ref = TypeHelper::getReferenceType(int_struct);

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=",
							  FunctionTypeInfo(BuiltIns::int_ref, {BuiltIns::int_ref, BuiltIns::int_struct}),
							  BuiltIns::int_struct,
							  {true, false, true}
    );

FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+",
							FunctionTypeInfo(BuiltIns::int_struct, {BuiltIns::int_struct, BuiltIns::int_struct}),
							BuiltIns::int_struct,
							{true, false, true}
    );

FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-",
							 FunctionTypeInfo(BuiltIns::int_struct, {BuiltIns::int_struct, BuiltIns::int_struct}),
							 BuiltIns::int_struct,
							 {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*",
						       FunctionTypeInfo(BuiltIns::int_struct, {BuiltIns::int_struct, BuiltIns::int_struct}),
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_constructor = new FunctionSymbol("int",
							       FunctionTypeInfo(int_ref, {int_ref, BuiltIns::int_type}),
							       BuiltIns::int_struct,
							       {true, true, false}
    );

FunctionSymbol *BuiltIns::int_default_constructor = new FunctionSymbol("int",
								       FunctionTypeInfo(int_ref, {int_ref}),
								       BuiltIns::int_struct,
								       {true, true, false}
    );

BuiltInTypeSymbol *BuiltIns::void_type = new BuiltInTypeSymbol("void", 0);
