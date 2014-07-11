#include "builtins.hpp"
#include "asmarraynode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

StructSymbol *BuiltIns::int_struct = new StructSymbol("int", BuiltIns::global_scope);

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=",
							  VariableType(BuiltIns::int_struct, true), 
							  {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct, true, true)},
							  BuiltIns::int_struct,
							  {true, false, true}
    );

FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+",
							VariableType(BuiltIns::int_struct), 
							{VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
							BuiltIns::int_struct,
							{true, false, true}
    );

FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-",
							 VariableType(BuiltIns::int_struct), 
							 {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
							 BuiltIns::int_struct,
							 {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*",
						       VariableType(BuiltIns::int_struct), 
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
    
FunctionSymbol *BuiltIns::int_eq = new FunctionSymbol("operator==",
						       VariableType(BuiltIns::int_struct), 
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_neq = new FunctionSymbol("operator!=",
						       VariableType(BuiltIns::int_struct), 
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_div = new FunctionSymbol("operator/",
						       VariableType(BuiltIns::int_struct), 
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mod = new FunctionSymbol("operator%",
						       VariableType(BuiltIns::int_struct), 
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
	
FunctionSymbol *BuiltIns::int_and = new FunctionSymbol("operator&&",
							   VariableType(BuiltIns::int_struct),
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_or = new FunctionSymbol("operator||",
							   VariableType(BuiltIns::int_struct),
							   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct)},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_default_constructor = new FunctionSymbol("int",
								       VariableType(BuiltIns::int_struct, true), 
									   {VariableType(BuiltIns::int_struct, true)},
								       BuiltIns::int_struct,
								       {true, true, false}
    );

FunctionSymbol *BuiltIns::int_copy_constructor = new FunctionSymbol("int",
									   VariableType(BuiltIns::int_struct, true), 
									   {VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::int_struct, true, true)},
									   BuiltIns::int_struct,
									   {true, true, false}
		);

BuiltInTypeSymbol *BuiltIns::void_type = new BuiltInTypeSymbol("void", 0);

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
	
StructSymbol *BuiltIns::char_struct = new StructSymbol("char", BuiltIns::global_scope);

FunctionSymbol *BuiltIns::char_assign = new FunctionSymbol("operator=",
														   VariableType(BuiltIns::char_struct, true),
														   {VariableType(BuiltIns::char_struct, true), VariableType(BuiltIns::char_struct, true, true)},
														   BuiltIns::char_struct,
														   {true, false, true}
		);

FunctionSymbol *BuiltIns::char_default_constructor = new FunctionSymbol("char",
																		VariableType(BuiltIns::char_struct, true),
																		{VariableType(BuiltIns::char_struct, true)},
																		BuiltIns::char_struct,
																		{true, true, false}
	   );

FunctionSymbol *BuiltIns::char_copy_constructor = new FunctionSymbol("char",
																	 VariableType(BuiltIns::char_struct, true),
																	 {VariableType(BuiltIns::char_struct, true), VariableType(BuiltIns::char_struct, true, true)},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::char_int_constructor = new FunctionSymbol("char",
																	 VariableType(BuiltIns::char_struct, true),
																	 {VariableType(BuiltIns::char_struct, true), VariableType(BuiltIns::int_struct, true, true)},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::putchar_func = new FunctionSymbol("putchar",
							    VariableType(BuiltIns::void_type), 
								{VariableType(BuiltIns::char_struct)},
							    BuiltIns::global_scope,
							    {false, false, false}
    );

FunctionSymbol *BuiltIns::getchar_func = new FunctionSymbol("getchar",
							    VariableType(BuiltIns::int_struct),
							   	{ },
							    BuiltIns::global_scope,
							    {false, false, false}
    );

StructSymbol *BuiltIns::ASCII_string = new StructSymbol("string", BuiltIns::global_scope);

FunctionSymbol *BuiltIns::ASCII_string_copy_constructor = new FunctionSymbol("string",
										VariableType(BuiltIns::ASCII_string, true),
										{VariableType(BuiltIns::ASCII_string, true), VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, true, false}
		);

FunctionSymbol *BuiltIns::ASCII_string_elem_operator = new FunctionSymbol("operator[]",
										VariableType(BuiltIns::char_struct, true),
										{VariableType(BuiltIns::ASCII_string, true), VariableType(BuiltIns::int_struct)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_length_func = new FunctionSymbol("length",
										VariableType(BuiltIns::int_struct),
										{VariableType(BuiltIns::ASCII_string, true)},
										BuiltIns::ASCII_string,
										{true, false, false}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_plus_operator = new FunctionSymbol("operator+",
										VariableType(BuiltIns::ASCII_string),
										{VariableType(BuiltIns::ASCII_string, true), VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);

FunctionSymbol *BuiltIns::ASCII_string_assign_operator = new FunctionSymbol("operator=",
										VariableType(BuiltIns::ASCII_string, true),
										{VariableType(BuiltIns::ASCII_string, true), VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
		
	
FunctionSymbol *BuiltIns::print_ASCII_string_func = new FunctionSymbol("print",
										VariableType(BuiltIns::void_type),
										{VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::global_scope,
										{false, false, false}
		);										
	
FunctionSymbol *BuiltIns::__fopen_func = new FunctionSymbol("__fopen",
	   									VariableType(BuiltIns::int_struct),
										{VariableType(BuiltIns::ASCII_string, true, true), VariableType(BuiltIns::int_struct), VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fclose_func = new FunctionSymbol("__fclose",
	   									VariableType(BuiltIns::void_type),
										{VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fwrite_func = new FunctionSymbol("__fwrite",
	   									VariableType(BuiltIns::int_struct),
										{VariableType(BuiltIns::int_struct), VariableType(BuiltIns::ASCII_string, true, true), VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::__fread_func = new FunctionSymbol("__fread",
	   									VariableType(BuiltIns::int_struct),
										{VariableType(BuiltIns::int_struct), VariableType(BuiltIns::ASCII_string, true), VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::int_char_constructor = new FunctionSymbol("int",
										VariableType(BuiltIns::int_struct, true),
										{VariableType(BuiltIns::int_struct, true), VariableType(BuiltIns::char_struct)},
										BuiltIns::int_struct,
										{true, true, false}
		);
