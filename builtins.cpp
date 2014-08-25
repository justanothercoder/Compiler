#include "builtins.hpp"
#include "asmarraynode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

StructSymbol *BuiltIns::int_struct = new StructSymbol("int", BuiltIns::global_scope, BuiltIns::global_scope -> getTemplateInfo());

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=",
							  new VariableType(BuiltIns::int_struct, true), 
							  {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct, true, true)},
							  BuiltIns::int_struct,
							  {true, false, true}
    );

FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+",
							new VariableType(BuiltIns::int_struct), 
							{new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
							BuiltIns::int_struct,
							{true, false, true}
    );

FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-",
							 new VariableType(BuiltIns::int_struct), 
							 {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
							 BuiltIns::int_struct,
							 {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*",
						       new VariableType(BuiltIns::int_struct), 
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
    
FunctionSymbol *BuiltIns::int_eq = new FunctionSymbol("operator==",
						       new VariableType(BuiltIns::int_struct), 
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_neq = new FunctionSymbol("operator!=",
						       new VariableType(BuiltIns::int_struct), 
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_div = new FunctionSymbol("operator/",
						       new VariableType(BuiltIns::int_struct), 
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mod = new FunctionSymbol("operator%",
						       new VariableType(BuiltIns::int_struct), 
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
	
FunctionSymbol *BuiltIns::int_and = new FunctionSymbol("operator&&",
							   new VariableType(BuiltIns::int_struct),
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_or = new FunctionSymbol("operator||",
							   new VariableType(BuiltIns::int_struct),
							   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct)},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_default_constructor = new FunctionSymbol("int",
								       new VariableType(BuiltIns::int_struct, true), 
									   {new VariableType(BuiltIns::int_struct, true)},
								       BuiltIns::int_struct,
								       {true, true, false}
    );

FunctionSymbol *BuiltIns::int_copy_constructor = new FunctionSymbol("int",
									   new VariableType(BuiltIns::int_struct, true), 
									   {new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::int_struct, true, true)},
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
	
StructSymbol *BuiltIns::char_struct = new StructSymbol("char", BuiltIns::global_scope, BuiltIns::global_scope -> getTemplateInfo());

FunctionSymbol *BuiltIns::char_assign = new FunctionSymbol("operator=",
														   new VariableType(BuiltIns::char_struct, true),
														   {new VariableType(BuiltIns::char_struct, true), new VariableType(BuiltIns::char_struct, true, true)},
														   BuiltIns::char_struct,
														   {true, false, true}
		);

FunctionSymbol *BuiltIns::char_default_constructor = new FunctionSymbol("char",
																		new VariableType(BuiltIns::char_struct, true),
																		{new VariableType(BuiltIns::char_struct, true)},
																		BuiltIns::char_struct,
																		{true, true, false}
	   );

FunctionSymbol *BuiltIns::char_copy_constructor = new FunctionSymbol("char",
																	 new VariableType(BuiltIns::char_struct, true),
																	 {new VariableType(BuiltIns::char_struct, true), new VariableType(BuiltIns::char_struct, true, true)},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::char_int_constructor = new FunctionSymbol("char",
																	 new VariableType(BuiltIns::char_struct, true),
																	 {new VariableType(BuiltIns::char_struct, true), new VariableType(BuiltIns::int_struct, true, true)},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::putchar_func = new FunctionSymbol("putchar",
							    new VariableType(BuiltIns::void_type), 
								{new VariableType(BuiltIns::char_struct)},
							    BuiltIns::global_scope,
							    {false, false, false}
    );

FunctionSymbol *BuiltIns::getchar_func = new FunctionSymbol("getchar",
							    new VariableType(BuiltIns::int_struct),
							   	{ },
							    BuiltIns::global_scope,
							    {false, false, false}
    );

StructSymbol *BuiltIns::ASCII_string = new StructSymbol("string", BuiltIns::global_scope, BuiltIns::global_scope -> getTemplateInfo());

FunctionSymbol *BuiltIns::ASCII_string_copy_constructor = new FunctionSymbol("string",
										new VariableType(BuiltIns::ASCII_string, true),
										{new VariableType(BuiltIns::ASCII_string, true), new VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, true, false}
		);

FunctionSymbol *BuiltIns::ASCII_string_elem_operator = new FunctionSymbol("operator[]",
										new VariableType(BuiltIns::char_struct, true),
										{new VariableType(BuiltIns::ASCII_string, true), new VariableType(BuiltIns::int_struct)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_length_func = new FunctionSymbol("length",
										new VariableType(BuiltIns::int_struct),
										{new VariableType(BuiltIns::ASCII_string, true)},
										BuiltIns::ASCII_string,
										{true, false, false}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_plus_operator = new FunctionSymbol("operator+",
										new VariableType(BuiltIns::ASCII_string),
										{new VariableType(BuiltIns::ASCII_string, true), new VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);

FunctionSymbol *BuiltIns::ASCII_string_assign_operator = new FunctionSymbol("operator=",
										new VariableType(BuiltIns::ASCII_string, true),
										{new VariableType(BuiltIns::ASCII_string, true), new VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
		
	
FunctionSymbol *BuiltIns::print_ASCII_string_func = new FunctionSymbol("print",
										new VariableType(BuiltIns::void_type),
										{new VariableType(BuiltIns::ASCII_string, true, true)},
										BuiltIns::global_scope,
										{false, false, false}
		);										
	
FunctionSymbol *BuiltIns::__fopen_func = new FunctionSymbol("__fopen",
	   									new VariableType(BuiltIns::int_struct),
										{new VariableType(BuiltIns::ASCII_string, true, true), new VariableType(BuiltIns::int_struct), new VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fclose_func = new FunctionSymbol("__fclose",
	   									new VariableType(BuiltIns::void_type),
										{new VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fwrite_func = new FunctionSymbol("__fwrite",
	   									new VariableType(BuiltIns::int_struct),
										{new VariableType(BuiltIns::int_struct), new VariableType(BuiltIns::ASCII_string, true, true), new VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::__fread_func = new FunctionSymbol("__fread",
	   									new VariableType(BuiltIns::int_struct),
										{new VariableType(BuiltIns::int_struct), new VariableType(BuiltIns::ASCII_string, true), new VariableType(BuiltIns::int_struct)},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::int_char_constructor = new FunctionSymbol("int",
										new VariableType(BuiltIns::int_struct, true),
										{new VariableType(BuiltIns::int_struct, true), new VariableType(BuiltIns::char_struct)},
										BuiltIns::int_struct,
										{true, true, false}
		);
