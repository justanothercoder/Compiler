#include "builtins.hpp"
#include "asmarraynode.hpp"
#include "templatestructdeclarationnode.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

BuiltInTypeSymbol *BuiltIns::int_type = new BuiltInTypeSymbol("~~int", GlobalConfig::int_size);

StructSymbol *BuiltIns::int_struct = new StructSymbol("int", BuiltIns::global_scope);

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=",
							  FunctionTypeInfo(VariableType(BuiltIns::int_struct, true, false), 
								  			  {VariableType(BuiltIns::int_struct, true, false), 
											   VariableType(BuiltIns::int_struct, false, false)}),
							  BuiltIns::int_struct,
							  {true, false, true}
    );

FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+",
							FunctionTypeInfo(VariableType(BuiltIns::int_struct, true, false), 
											{VariableType(BuiltIns::int_struct, true, false), 
											 VariableType(BuiltIns::int_struct, false, false)}),
							BuiltIns::int_struct,
							{true, false, true}
    );

FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-",
							 FunctionTypeInfo(VariableType(BuiltIns::int_struct, false, false), 
								 			 {VariableType(BuiltIns::int_struct, true, false), 
											  VariableType(BuiltIns::int_struct, false, false)}),
							 BuiltIns::int_struct,
							 {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*",
						       FunctionTypeInfo(VariableType(BuiltIns::int_struct, false, false), 
								   			   {VariableType(BuiltIns::int_struct, true, false), 
											    VariableType(BuiltIns::int_struct, false, false)}),
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_default_constructor = new FunctionSymbol("int",
								       FunctionTypeInfo(VariableType(BuiltIns::int_struct, true, false), 
										               {VariableType(BuiltIns::int_struct, true, false)}),
								       BuiltIns::int_struct,
								       {true, true, false}
    );

FunctionSymbol *BuiltIns::int_copy_constructor = new FunctionSymbol("int",
									   FunctionTypeInfo(VariableType(BuiltIns::int_struct, true, false), 
										   			   {VariableType(BuiltIns::int_struct, true, false), 
													    VariableType(BuiltIns::int_struct, false, false)}),
									   BuiltIns::int_struct,
									   {true, true, false}
		);

BuiltInTypeSymbol *BuiltIns::void_type = new BuiltInTypeSymbol("void", 0);

FunctionSymbol *BuiltIns::putchar_func = new FunctionSymbol("putchar",
							    FunctionTypeInfo(VariableType(BuiltIns::void_type, false, false), 
												{VariableType(BuiltIns::int_struct, false, false)}),
							    BuiltIns::global_scope,
							    {false, false, false}
    );

FunctionSymbol *BuiltIns::getchar_func = new FunctionSymbol("getchar",
							    FunctionTypeInfo(VariableType(BuiltIns::int_struct, false, false), { }),
							    BuiltIns::global_scope,
							    {false, false, false}
    );

TemplateStructSymbol *BuiltIns::array_struct = new TemplateStructSymbol("array",
																		BuiltIns::global_scope,
																		{ {"T", TypeInfo("class", false, { }) }, 
																		  { "size", TypeInfo("int", false, { }) }
																		},
																		new TemplateStructDeclarationNode(
																			"array",
																			{new AsmArrayNode()},
																			{ {"T", TypeInfo("class", false, { }) }, 
																			  { "size", TypeInfo("int", false, { }) }
																			}
																		)
		);	
