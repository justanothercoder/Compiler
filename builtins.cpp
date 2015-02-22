#include "builtins.hpp"
#include "globalscope.hpp"
#include "typefactory.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "builtintypesymbol.hpp"
#include "symbolfactory.hpp"
#include "typesymbol.hpp"
#include "functiontype.hpp"
#include "structscope.hpp"

std::shared_ptr<Scope> BuiltIns::global_scope;
const Type* BuiltIns::void_type;
const Type* BuiltIns::int_type;
const Type* BuiltIns::char_type;
const Type* BuiltIns::ASCII_string_type;

void BuiltIns::defineBuiltIns()
{
    auto factory = SymbolFactory{ };

    auto makeRef = [](auto&& type, bool is_const) { return VariableType(TypeFactory::getReference(type.get()), is_const); };

    auto global_scope = std::make_shared<GlobalScope>();
   
    auto int_          = factory.makeStruct("int", new StructScope("int", global_scope.get()));
    auto ref_int       = makeRef(int_, false);
    auto const_ref_int = makeRef(int_, true);

    auto char_          = factory.makeStruct("char", new StructScope("char", global_scope.get()));
    auto ref_char       = makeRef(char_, false);
    auto const_ref_char = makeRef(char_, true);

    auto str           = factory.makeStruct("string", new StructScope("string", global_scope.get()));
    auto ref_str       = makeRef(str, false);
    auto const_ref_str = makeRef(str, true);

    auto int_builtin  = VariableType(new BuiltInTypeSymbol("~~int", Comp::config().int_size), false);
    auto char_builtin = VariableType(new BuiltInTypeSymbol("~~char", Comp::config().int_size), false);

    auto simple_traits = FunctionTraits::simple();
    auto op_traits     = FunctionTraits::methodOper();
    auto meth_traits   = FunctionTraits::method();
    auto constr_traits = FunctionTraits::constructor();

    auto tp = FunctionType(int_.get(), {ref_int, int_.get()});

    auto void_type = std::make_unique<BuiltInTypeSymbol>("void", 0);

    global_scope -> define(factory.makeFunction("putchar", FunctionType(void_type.get(), {char_.get()}), simple_traits, false));
    global_scope -> define(factory.makeFunction("getchar", FunctionType(int_.get(), { }), simple_traits, false));

    auto string_builtin = VariableType(new BuiltInTypeSymbol("~~string", 256), false);    
    str -> defineMember(factory.makeVariable("~~impl", string_builtin, VariableSymbolType::FIELD));

    auto str_tp = FunctionType(ref_str, {ref_str, const_ref_str});
    
    int_ -> defineMember(factory.makeVariable("~~impl", int_builtin, VariableSymbolType::FIELD));
    
    int_ -> defineMethod(factory.makeFunction("int", FunctionType(ref_int, {ref_int}), constr_traits, false));
    int_ -> defineMethod(factory.makeFunction("int", FunctionType(ref_int, {ref_int, const_ref_int}), constr_traits, false));
    
    int_ -> defineMethod(factory.makeFunction("operator=", FunctionType(ref_int, {ref_int, const_ref_int}), op_traits, false));

    int_ -> defineMethod(factory.makeFunction("operator+", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator-", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator*", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator/", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator%", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator==", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator!=", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator&&", tp, op_traits, false));
    int_ -> defineMethod(factory.makeFunction("operator||", tp, op_traits, false));

    int_ -> defineMethod(factory.makeFunction("int", FunctionType(ref_int, {ref_int, char_.get()}), constr_traits, false));

    char_ -> defineMember(factory.makeVariable("~~impl", char_builtin, VariableSymbolType::FIELD));

    char_ -> defineMethod(factory.makeFunction("char", FunctionType(ref_char, {ref_char}), constr_traits, false));
    char_ -> defineMethod(factory.makeFunction("char", FunctionType(ref_char, {ref_char, const_ref_char}), constr_traits, false));
    char_ -> defineMethod(factory.makeFunction("char", FunctionType(ref_char, {ref_char, const_ref_int}), constr_traits, false));

    char_ -> defineMethod(factory.makeFunction("operator=", FunctionType(ref_char, {ref_char, const_ref_char}), constr_traits, false));

    str -> defineMethod(factory.makeFunction("string", str_tp, constr_traits, false));
    str -> defineMethod(factory.makeFunction("length", FunctionType(int_.get(), {ref_str}), meth_traits, false));

    str -> defineMethod(factory.makeFunction("operator[]", FunctionType(ref_char, {ref_str, int_.get()}), op_traits, false));
    str -> defineMethod(factory.makeFunction("operator+", FunctionType(str.get(), {ref_str, const_ref_str}), op_traits, false));
    str -> defineMethod(factory.makeFunction("operator=", str_tp, op_traits, false));

    global_scope -> define(factory.makeFunction("print", FunctionType(void_type.get(), {const_ref_str}), simple_traits, false));
    
    BuiltIns::int_type          = int_.get();
    BuiltIns::char_type         = char_.get();
    BuiltIns::ASCII_string_type = str.get();
    BuiltIns::void_type         = void_type.get();
    
    global_scope -> define(std::move(void_type));
    global_scope -> define(std::move(int_));
    global_scope -> define(std::move(str));
    global_scope -> define(std::move(char_));
    
    BuiltIns::global_scope      = global_scope;
}

bool isIntType(const Type* t)    { return t == BuiltIns::int_type; }
bool isCharType(const Type* t)   { return t == BuiltIns::char_type; }
bool isPointer(const Type* t)    { return t -> isPointer(); }
bool isReference(const Type* t)  { return t -> isReference(); }
bool isSimpleType(const Type* t) { return isIntType(t) || isCharType(t) || isPointer(t) || isReference(t); }
