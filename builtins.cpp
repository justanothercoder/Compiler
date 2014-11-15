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

    StructSymbol *int_struct = new StructSymbol("int", global_scope, global_scope -> getTemplateInfo());

    BuiltIns::int_type = int_struct;

    const Type *ref_int       = TypeFactory::getReference(int_struct);
    const Type *const_ref_int = TypeFactory::getConst(ref_int);

    StructSymbol *char_struct = new StructSymbol("char", global_scope, global_scope -> getTemplateInfo());

    BuiltIns::char_type = char_struct;

    const Type *ref_char       = TypeFactory::getReference(char_struct);
    const Type *const_ref_char = TypeFactory::getConst(ref_char);

    StructSymbol *ASCII_string = new StructSymbol("string", global_scope, global_scope -> getTemplateInfo());

    ASCII_string_type = ASCII_string;

    const Type *ref_ASCII_string       = TypeFactory::getReference(ASCII_string);
    const Type *const_ref_ASCII_string = TypeFactory::getConst(ref_ASCII_string);

    global_scope -> define(int_struct);
    global_scope -> define(ASCII_string);
    global_scope -> define(char_struct);

    FunctionTraits simple_traits      = {false, false, false};
    FunctionTraits method_traits      = {true, false, false};
    FunctionTraits method_oper_traits = {true, false, true};
    FunctionTraits constructor_traits = {true, true, false};

    int_struct  -> define(new VariableSymbol("~~impl", new BuiltInTypeSymbol("~~int", GlobalConfig::int_size), VariableSymbolType::FIELD));
    char_struct -> define(new VariableSymbol("~~impl", new BuiltInTypeSymbol("~~char", GlobalConfig::int_size), VariableSymbolType::FIELD));

    int_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_int, {ref_int}));
    int_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_int, {ref_int, const_ref_int}));

    auto tp = TypeFactory::getFunctionType(int_struct, {ref_int, int_struct});

    int_struct -> defineBuiltInOperator("operator=", TypeFactory::getFunctionType(ref_int, {ref_int, const_ref_int}));

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

    char_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_char, {ref_char}));
    char_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_char, {ref_char, const_ref_char}));
    char_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_char, {ref_char, const_ref_int}));

    char_struct -> defineBuiltInOperator("operator=", TypeFactory::getFunctionType(ref_char, {ref_char, const_ref_char}));

    int_struct -> defineBuiltInConstructor(TypeFactory::getFunctionType(ref_int, {ref_int, char_struct}));

    BuiltInTypeSymbol *void_type = new BuiltInTypeSymbol("void", 0);
    auto void_ptr = TypeFactory::getPointer(void_type);

    BuiltIns::void_type = void_type;

    global_scope -> define(const_cast<Symbol*>(BuiltIns::void_type -> getSymbol()));

    global_scope -> defineBuiltInFunction("putchar", TypeFactory::getFunctionType(void_type, {char_struct}));    
    global_scope -> defineBuiltInFunction("getchar", TypeFactory::getFunctionType(int_struct, { }));

    ASCII_string -> define(new VariableSymbol("~~impl", new BuiltInTypeSymbol("~~string", 256), VariableSymbolType::FIELD));

    auto str_tp = TypeFactory::getFunctionType(ref_ASCII_string, {ref_ASCII_string, const_ref_ASCII_string});

    ASCII_string -> defineBuiltInConstructor(str_tp);
    ASCII_string -> defineBuiltInMethod("length", TypeFactory::getFunctionType(int_struct, {ref_ASCII_string}));

    ASCII_string -> defineBuiltInOperator("operator[]", TypeFactory::getFunctionType(ref_char, {ref_ASCII_string, int_struct}));
    ASCII_string -> defineBuiltInOperator("operator+", TypeFactory::getFunctionType(ASCII_string, {ref_ASCII_string, const_ref_ASCII_string}));
    ASCII_string -> defineBuiltInOperator("operator=", str_tp);

    Logger::log(TypeFactory::getFunctionType(int_struct, {int_struct, const_ref_ASCII_string, int_struct}) -> getName());
    
    global_scope -> defineBuiltInFunction("print" , TypeFactory::getFunctionType(void_type, {const_ref_ASCII_string}));
    global_scope -> defineBuiltInFunction("__fopen", TypeFactory::getFunctionType(int_struct, {const_ref_ASCII_string, int_struct, int_struct}));
    global_scope -> defineBuiltInFunction("__fclose", TypeFactory::getFunctionType(void_type, {int_struct}));
    global_scope -> defineBuiltInFunction("__fwrite", TypeFactory::getFunctionType(int_struct, {int_struct, const_ref_ASCII_string, int_struct}));
    global_scope -> defineBuiltInFunction("__fread", TypeFactory::getFunctionType(int_struct, {int_struct, ref_ASCII_string, int_struct}));
    global_scope -> defineBuiltInFunction("__brk", TypeFactory::getFunctionType(void_ptr, {void_ptr}));

    int_struct -> is_defined = true;
    ASCII_string -> is_defined = true;
    char_struct -> is_defined = true;

    global_scope -> resolve("putchar") -> is_defined = true;
    global_scope -> resolve("getchar") -> is_defined = true;
    global_scope -> resolve("print") -> is_defined   = true;

    global_scope -> resolve("__fopen") -> is_defined  = true;
    global_scope -> resolve("__fclose") -> is_defined = true;
    global_scope -> resolve("__fwrite") -> is_defined = true;
    global_scope -> resolve("__fread") -> is_defined  = true;

    global_scope -> resolve("__brk") -> is_defined  = true;
}

