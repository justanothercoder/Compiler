#include "builtins.hpp"
#include "globalscope.hpp"
#include "typefactory.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "structsymbol.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "builtintypesymbol.hpp"

std::shared_ptr<Scope> BuiltIns::global_scope;
std::shared_ptr<Type> BuiltIns::void_type;
std::shared_ptr<Type> BuiltIns::int_type;
std::shared_ptr<Type> BuiltIns::char_type;
std::shared_ptr<Type> BuiltIns::ASCII_string_type;

void BuiltIns::defineBuiltIns()
{
    auto global_scope = std::make_shared<GlobalScope>();
    BuiltIns::global_scope = global_scope;

    auto int_struct = std::make_shared<StructSymbol>("int", global_scope.get());
    BuiltIns::int_type = int_struct;

    auto ref_int       = VariableType(TypeFactory::getReference(int_struct.get()), false);
    auto const_ref_int = VariableType(TypeFactory::getReference(int_struct.get()), true);

    auto char_struct = std::make_shared<StructSymbol>("char", global_scope.get());
    BuiltIns::char_type = char_struct;

    auto ref_char       = VariableType(TypeFactory::getReference(char_struct.get()), false);
    auto const_ref_char = VariableType(TypeFactory::getReference(char_struct.get()), true);

    auto ASCII_string = std::make_shared<StructSymbol>("string", global_scope.get());
    ASCII_string_type = ASCII_string;

    auto ref_ASCII_string       = VariableType(TypeFactory::getReference(ASCII_string.get()), false);
    auto const_ref_ASCII_string = VariableType(TypeFactory::getReference(ASCII_string.get()), true);

    auto int_builtin = VariableType(new BuiltInTypeSymbol("~~int", Comp::config().int_size), false);

    auto char_builtin = VariableType(new BuiltInTypeSymbol("~~char", Comp::config().int_size), false);

    auto tp = FunctionType(int_struct.get(), {ref_int, int_struct.get()});

//	int_plus  -> is_constexpr = true;
//	int_minus -> is_constexpr = true;

    auto void_type = std::make_shared<BuiltInTypeSymbol>("void", 0);
    BuiltIns::void_type = void_type;

    global_scope -> defineBuiltInFunction("putchar", FunctionType(void_type.get(), {char_struct.get()}));    
    global_scope -> defineBuiltInFunction("getchar", FunctionType(int_struct.get(), { }));

    auto string_builtin = VariableType(new BuiltInTypeSymbol("~~string", 256), false);
    ASCII_string -> define(std::make_unique<VariableSymbol>("~~impl", string_builtin, VariableSymbolType::FIELD));

    auto str_tp = FunctionType(ref_ASCII_string, {ref_ASCII_string, const_ref_ASCII_string});
    
    int_struct  -> define(std::make_unique<VariableSymbol>("~~impl", int_builtin, VariableSymbolType::FIELD));
    
    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int}));
    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int, const_ref_int}));
    
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

    int_struct -> defineBuiltInConstructor(FunctionType(ref_int, {ref_int, char_struct.get()}));

    char_struct -> define(std::make_unique<VariableSymbol>("~~impl", char_builtin, VariableSymbolType::FIELD));

    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char}));
    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char, const_ref_char}));
    char_struct -> defineBuiltInConstructor(FunctionType(ref_char, {ref_char, const_ref_int}));

    char_struct -> defineBuiltInOperator("operator=", FunctionType(ref_char, {ref_char, const_ref_char}));

    ASCII_string -> defineBuiltInConstructor(str_tp);
    ASCII_string -> defineBuiltInMethod("length", FunctionType(int_struct.get(), {ref_ASCII_string}));

    ASCII_string -> defineBuiltInOperator("operator[]", FunctionType(ref_char, {ref_ASCII_string, int_struct.get()}));
    ASCII_string -> defineBuiltInOperator("operator+", FunctionType(ASCII_string.get(), {ref_ASCII_string, const_ref_ASCII_string}));
    ASCII_string -> defineBuiltInOperator("operator=", str_tp);

    global_scope -> defineBuiltInFunction("print" , FunctionType(void_type.get(), {const_ref_ASCII_string}));
    
    global_scope -> define(std::move(void_type));
    global_scope -> define(std::move(int_struct));
    global_scope -> define(std::move(ASCII_string));
    global_scope -> define(std::move(char_struct));
    
}

bool isIntType(const Type* t)    { return t == BuiltIns::int_type.get(); }
bool isCharType(const Type* t)   { return t == BuiltIns::char_type.get(); }
bool isPointer(const Type* t)    { return t -> isPointer(); }
bool isReference(const Type* t)  { return t -> isReference(); }
bool isSimpleType(const Type* t) { return isIntType(t) || isCharType(t) || isPointer(t) || isReference(t); }
