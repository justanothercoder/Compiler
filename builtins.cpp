#include "builtins.hpp"
#include "templatestructdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"
#include "typefactory.hpp"
#include "logger.hpp"

Scope *BuiltIns::global_scope;
Type *BuiltIns::void_type;
Type *BuiltIns::int_type;
Type *BuiltIns::char_type;
Type *BuiltIns::ASCII_string_type;

void BuiltIns::defineBuiltIns()
{
    auto global_scope = new GlobalScope();

    BuiltIns::global_scope = global_scope;

    StructSymbol *int_struct = new StructSymbol("int", global_scope, global_scope -> templateInfo());

    BuiltIns::int_type = int_struct;

    auto ref_int       = VariableType(TypeFactory::getReference(int_struct), false);
    auto const_ref_int = VariableType(TypeFactory::getReference(int_struct), true);

    StructSymbol *char_struct = new StructSymbol("char", global_scope, global_scope -> templateInfo());

    BuiltIns::char_type = char_struct;

    auto ref_char       = VariableType(TypeFactory::getReference(char_struct), false);
    auto const_ref_char = VariableType(TypeFactory::getReference(char_struct), true);

    StructSymbol *ASCII_string = new StructSymbol("string", global_scope, global_scope -> templateInfo());

    ASCII_string_type = ASCII_string;

    auto ref_ASCII_string       = VariableType(TypeFactory::getReference(ASCII_string), false);
    auto const_ref_ASCII_string = VariableType(TypeFactory::getReference(ASCII_string), true);

    global_scope -> define(int_struct);
    global_scope -> define(ASCII_string);
    global_scope -> define(char_struct);

    auto int_builtin = VariableType(new BuiltInTypeSymbol("~~int", GlobalConfig::int_size), false);
    int_struct  -> define(new VariableSymbol("~~impl", int_builtin, VariableSymbolType::FIELD));

    auto char_builtin = VariableType(new BuiltInTypeSymbol("~~char", GlobalConfig::int_size), false);
    char_struct -> define(new VariableSymbol("~~impl", char_builtin, VariableSymbolType::FIELD));

    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int}));
    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int, const_ref_int}));

    auto tp = FunctionType(int_struct, {ref_int, int_struct});

    int_struct -> defineBuiltInOperator("operator=", FunctionType(ref_int, {ref_int, const_ref_int}));

    int_struct -> defineBuiltInOperator("operator+", tp);
    int_struct -> defineBuiltInOperator("operator-", tp);
    int_struct -> defineBuiltInOperator("operator*", tp);
    int_struct -> defineBuiltInOperator("operator/", tp);
    int_struct -> defineBuiltInOperator("operator%", tp);
    int_struct -> defineBuiltInOperator("operator==", tp);
    int_struct -> defineBuiltInOperator("operator!=", tp);
    int_struct -> defineBuiltInOperator("operator&&", tp);
    int_struct -> defineBuiltInOperator("operator||", tp);

//	int_plus  -> is_constexpr = true;
//	int_minus -> is_constexpr = true;

    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char}));
    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char, const_ref_char}));
    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char, const_ref_int}));

    char_struct -> defineBuiltInOperator("operator=", FunctionType(ref_char, {ref_char, const_ref_char}));

    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int, char_struct}));

    BuiltInTypeSymbol *void_type = new BuiltInTypeSymbol("void", 0);
    BuiltIns::void_type = void_type;

//    global_scope -> define(const_cast<Symbol*>(BuiltIns::void_type -> getSymbol()));
    global_scope -> define(void_type);

    global_scope -> defineBuiltInFunction("putchar", FunctionType(void_type, {char_struct}));    
    global_scope -> defineBuiltInFunction("getchar", FunctionType(int_struct, { }));

    auto string_builtin = VariableType(new BuiltInTypeSymbol("~~string", 256), false);
    ASCII_string -> define(new VariableSymbol("~~impl", string_builtin, VariableSymbolType::FIELD));

    auto str_tp = FunctionType(ref_ASCII_string, {ref_ASCII_string, const_ref_ASCII_string});

    ASCII_string -> defineBuiltInConstructor(str_tp);
    ASCII_string -> defineBuiltInMethod("length", FunctionType(int_struct, {ref_ASCII_string}));

    ASCII_string -> defineBuiltInOperator("operator[]", FunctionType(ref_char, {ref_ASCII_string, int_struct}));
    ASCII_string -> defineBuiltInOperator("operator+", FunctionType(ASCII_string, {ref_ASCII_string, const_ref_ASCII_string}));
    ASCII_string -> defineBuiltInOperator("operator=", str_tp);

    global_scope -> defineBuiltInFunction("print" , FunctionType(void_type, {const_ref_ASCII_string}));

    int_struct -> is_defined = true;
    ASCII_string -> is_defined = true;
    char_struct -> is_defined = true;

    global_scope -> resolve("putchar") -> is_defined = true;
    global_scope -> resolve("getchar") -> is_defined = true;
    global_scope -> resolve("print") -> is_defined   = true;
}

