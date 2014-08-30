#include "builtins.hpp"
#include "asmarraynode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"
#include "typefactory.hpp"

Scope *BuiltIns::global_scope = new GlobalScope();

StructSymbol *BuiltIns::int_struct = new StructSymbol("int", BuiltIns::global_scope, BuiltIns::global_scope -> getTemplateInfo());

const Type *ref_int       = TypeFactory::getReference(BuiltIns::int_struct);
const Type *const_ref_int = TypeFactory::getConst(ref_int);

FunctionSymbol *BuiltIns::int_assign = new FunctionSymbol("operator=",
							  ref_int, 
							  {ref_int, const_ref_int},
							  BuiltIns::int_struct,
							  {true, false, true}
    );

FunctionSymbol *BuiltIns::int_plus = new FunctionSymbol("operator+",
							BuiltIns::int_struct, 
							{ref_int, BuiltIns::int_struct},
							BuiltIns::int_struct,
							{true, false, true}
    );

FunctionSymbol *BuiltIns::int_minus = new FunctionSymbol("operator-",
							 BuiltIns::int_struct, 
							 {ref_int, BuiltIns::int_struct},
							 BuiltIns::int_struct,
							 {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mul = new FunctionSymbol("operator*",
						       BuiltIns::int_struct, 
							   {ref_int, BuiltIns::int_struct},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
    
FunctionSymbol *BuiltIns::int_eq = new FunctionSymbol("operator==",
						       BuiltIns::int_struct, 
							   {ref_int, BuiltIns::int_struct},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_neq = new FunctionSymbol("operator!=",
						       BuiltIns::int_struct,
							   {ref_int, BuiltIns::int_struct},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_div = new FunctionSymbol("operator/",
						       BuiltIns::int_struct, 
							   {ref_int, BuiltIns::int_struct},
						       BuiltIns::int_struct,
						       {true, false, true}
    );

FunctionSymbol *BuiltIns::int_mod = new FunctionSymbol("operator%",
						       BuiltIns::int_struct, 
							   {ref_int, BuiltIns::int_struct},
						       BuiltIns::int_struct,
						       {true, false, true}
    );
	
FunctionSymbol *BuiltIns::int_and = new FunctionSymbol("operator&&",
							   BuiltIns::int_struct,
							   {ref_int, BuiltIns::int_struct},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_or = new FunctionSymbol("operator||",
							   BuiltIns::int_struct,
							   {ref_int, BuiltIns::int_struct},
							   BuiltIns::int_struct,
							   {true, false, true}
		);

FunctionSymbol *BuiltIns::int_default_constructor = new FunctionSymbol("int", ref_int, {ref_int}, BuiltIns::int_struct, {true, true, false});

FunctionSymbol *BuiltIns::int_copy_constructor = new FunctionSymbol("int",
									   ref_int, 
									   {ref_int, const_ref_int},
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

const Type *ref_char       = TypeFactory::getReference(BuiltIns::char_struct);
const Type *const_ref_char = TypeFactory::getConst(ref_char);

FunctionSymbol *BuiltIns::char_assign = new FunctionSymbol("operator=",
														   ref_char,
														   {ref_char, const_ref_char},
														   BuiltIns::char_struct,
														   {true, false, true}
		);

FunctionSymbol *BuiltIns::char_default_constructor = new FunctionSymbol("char",
																		ref_char,
																		{ref_char},
																		BuiltIns::char_struct,
																		{true, true, false}
	   );

FunctionSymbol *BuiltIns::char_copy_constructor = new FunctionSymbol("char",
																	 ref_char,
																	 {ref_char, const_ref_char},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::char_int_constructor = new FunctionSymbol("char",
																	 ref_char,
																	 {ref_char, const_ref_int},
																	 BuiltIns::char_struct,
																	 {true, true, false}
	  );

FunctionSymbol *BuiltIns::putchar_func = new FunctionSymbol("putchar",
							    BuiltIns::void_type, 
								{BuiltIns::char_struct},
							    BuiltIns::global_scope,
							    {false, false, false}
    );

FunctionSymbol *BuiltIns::getchar_func = new FunctionSymbol("getchar",
							    BuiltIns::int_struct,
							   	{ },
							    BuiltIns::global_scope,
							    {false, false, false}
    );

StructSymbol *BuiltIns::ASCII_string = new StructSymbol("string", BuiltIns::global_scope, BuiltIns::global_scope -> getTemplateInfo());

const Type *ref_ASCII_string       = TypeFactory::getReference(BuiltIns::ASCII_string);
const Type *const_ref_ASCII_string = TypeFactory::getConst(ref_ASCII_string);

FunctionSymbol *BuiltIns::ASCII_string_copy_constructor = new FunctionSymbol("string",
										ref_ASCII_string,
										{ref_ASCII_string, const_ref_ASCII_string},
										BuiltIns::ASCII_string,
										{true, true, false}
		);

FunctionSymbol *BuiltIns::ASCII_string_elem_operator = new FunctionSymbol("operator[]",
										ref_char,
										{ref_ASCII_string, BuiltIns::int_struct},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_length_func = new FunctionSymbol("length",
										BuiltIns::int_struct,
										{ref_ASCII_string},
										BuiltIns::ASCII_string,
										{true, false, false}
		);
	
FunctionSymbol *BuiltIns::ASCII_string_plus_operator = new FunctionSymbol("operator+",
										BuiltIns::ASCII_string,
										{ref_ASCII_string, const_ref_ASCII_string},
										BuiltIns::ASCII_string,
										{true, false, true}
		);

FunctionSymbol *BuiltIns::ASCII_string_assign_operator = new FunctionSymbol("operator=",
										ref_ASCII_string,
										{ref_ASCII_string, const_ref_ASCII_string},
										BuiltIns::ASCII_string,
										{true, false, true}
		);
		
	
FunctionSymbol *BuiltIns::print_ASCII_string_func = new FunctionSymbol("print",
										BuiltIns::void_type,
										{const_ref_ASCII_string},
										BuiltIns::global_scope,
										{false, false, false}
		);										
	
FunctionSymbol *BuiltIns::__fopen_func = new FunctionSymbol("__fopen",
	   									BuiltIns::int_struct,
										{const_ref_ASCII_string, BuiltIns::int_struct, BuiltIns::int_struct},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fclose_func = new FunctionSymbol("__fclose",
	   									BuiltIns::void_type,
										{BuiltIns::int_struct},
										BuiltIns::global_scope,
										{false, false, false}
		);

FunctionSymbol *BuiltIns::__fwrite_func = new FunctionSymbol("__fwrite",
	   									BuiltIns::int_struct,
										{BuiltIns::int_struct, const_ref_ASCII_string, BuiltIns::int_struct},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::__fread_func = new FunctionSymbol("__fread",
	   									BuiltIns::int_struct,
										{BuiltIns::int_struct, ref_ASCII_string, BuiltIns::int_struct},
										BuiltIns::global_scope,
										{false, false, false}
		); 

FunctionSymbol *BuiltIns::int_char_constructor = new FunctionSymbol("int",
										ref_int,
										{ref_int, BuiltIns::char_struct},
										BuiltIns::int_struct,
										{true, true, false}
		);
