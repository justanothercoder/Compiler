#include "builtins.hpp"
#include "templatestructdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"
#include "typefactory.hpp"

Scope *BuiltIns::global_scope;
Type *BuiltIns::void_type;
Type *BuiltIns::int_type;
Type *BuiltIns::char_type;
Type *BuiltIns::ASCII_string_type;

void BuiltIns::defineBuiltIns()
{
    Scope *global_scope = new GlobalScope();

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

    int_struct -> define(new FunctionSymbol("int", ref_int, {ref_int}, int_struct, constructor_traits));
    int_struct -> define(new FunctionSymbol("int", ref_int, {ref_int, const_ref_int}, int_struct, constructor_traits));

    int_struct -> define(new FunctionSymbol("operator=" , ref_int, {ref_int, const_ref_int}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator+" , int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator-" , int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator*" , int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator==", int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator!=", int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator/" , int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator%" , int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator&&", int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));
    int_struct -> define(new FunctionSymbol("operator||", int_struct, {ref_int, int_struct}, int_struct, method_oper_traits));

//	int_plus  -> is_constexpr = true;
//	int_minus -> is_constexpr = true;

    char_struct -> define(new FunctionSymbol("char", ref_char, {ref_char}, char_struct, constructor_traits));
    char_struct -> define(new FunctionSymbol("char", ref_char, {ref_char, const_ref_char}, char_struct, constructor_traits));
    char_struct -> define(new FunctionSymbol("char", ref_char, {ref_char, const_ref_int}, char_struct, constructor_traits));
    char_struct -> define(new FunctionSymbol("operator=", ref_char, {ref_char, const_ref_char}, char_struct, method_oper_traits));

    int_struct -> define(new FunctionSymbol("int", ref_int, {ref_int, char_struct}, int_struct, constructor_traits));

    BuiltInTypeSymbol *void_type = new BuiltInTypeSymbol("void", 0);
    auto void_ptr = TypeFactory::getPointer(void_type);

    BuiltIns::void_type = void_type;

    global_scope -> define(const_cast<Symbol*>(BuiltIns::void_type -> getSymbol()));
    global_scope -> define(new FunctionSymbol("putchar", void_type, {char_struct}, global_scope, simple_traits));
    global_scope -> define(new FunctionSymbol("getchar", int_struct, { }, global_scope, simple_traits));

    ASCII_string -> define(new VariableSymbol("~~impl", new BuiltInTypeSymbol("~~string", 256), VariableSymbolType::FIELD));

    ASCII_string -> define(new FunctionSymbol("string", ref_ASCII_string, {ref_ASCII_string, const_ref_ASCII_string}, ASCII_string, constructor_traits));
    ASCII_string -> define(new FunctionSymbol("operator[]", ref_char, {ref_ASCII_string, int_struct}, ASCII_string, method_oper_traits));
    ASCII_string -> define(new FunctionSymbol("length", int_struct, {ref_ASCII_string}, ASCII_string, method_traits));
    ASCII_string -> define(new FunctionSymbol("operator+", ASCII_string, {ref_ASCII_string, const_ref_ASCII_string}, ASCII_string, method_oper_traits));
    ASCII_string -> define(new FunctionSymbol("operator=", ref_ASCII_string, {ref_ASCII_string, const_ref_ASCII_string}, ASCII_string, method_oper_traits));

    global_scope -> define(new FunctionSymbol("print", void_type, {const_ref_ASCII_string}, global_scope, simple_traits));

    global_scope -> define(new FunctionSymbol("__fopen", int_struct, {const_ref_ASCII_string, int_struct, int_struct}, global_scope, simple_traits));
    global_scope -> define(new FunctionSymbol("__fclose", void_type, {int_struct}, global_scope, simple_traits));
    global_scope -> define(new FunctionSymbol("__fwrite", int_struct, {int_struct, const_ref_ASCII_string, int_struct}, global_scope, simple_traits));
    global_scope -> define(new FunctionSymbol("__fread", int_struct, {int_struct, ref_ASCII_string, int_struct}, global_scope, simple_traits));

    global_scope -> define(new FunctionSymbol("__brk", void_ptr, {void_ptr}, global_scope, simple_traits));

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

