#include "typefactory.hpp"

#include "pointertype.hpp"
#include "referencetype.hpp"
#include "arraytype.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"
#include "globalscope.hpp"

#include "globaltable.hpp"

const Type* TypeFactory::getPointer(const Type *type)
{
    static std::map<const Type*, const Type*> pointers;

    if ( type -> isReference() )
        return nullptr;

    auto it = pointers.find(type);

    if ( it == std::end(pointers) )
    {
        pointers[type] = new PointerType(type);
        const auto& _tp = pointers[type];

        auto tp = VariableType(_tp, false);
        auto tp_ref = VariableType(getReference(_tp), false);
        auto const_tp_ref = VariableType(getReference(_tp), true);            
    
        auto nonconst_int = VariableType(BuiltIns::int_type.get(), false);

        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator=", FunctionType(tp_ref, {tp_ref, const_tp_ref}));
        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator+", FunctionType(tp, {tp, nonconst_int}));
    }

    return pointers[type];
}

const Type* TypeFactory::getReference(const Type *type)
{
    static std::map<const Type*, const Type*> references;

    if ( type -> isReference() )
        return nullptr;

    auto it = references.find(type);

    if ( it == std::end(references) )
        references[type] = new ReferenceType(type);

    return references[type];
}

const Type* TypeFactory::getArray(const Type *type, int size)
{
    static std::map<const Type*, std::map<int, const Type*> > arrays;

    if ( type -> isReference() )
        return nullptr;

    auto it = arrays.find(type);    

    if ( it == std::end(arrays) || it -> second.find(size) == std::end(it -> second) )
    {
        arrays[type][size] = new ArrayType(type, size);

        const auto& _tp = arrays[type][size];

        auto tp = VariableType(_tp, false);
        auto tp_ref = VariableType(getReference(_tp), false);
        auto const_tp_ref = VariableType(getReference(_tp), true);

        auto type_ptr = VariableType(getPointer(type), false);
        auto type_ref = VariableType(getReference(type), false);

        auto nonconst_int = VariableType(BuiltIns::int_type.get(), false);

        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator=", FunctionType(tp_ref, {tp_ref, const_tp_ref}));
        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator+", FunctionType(type_ptr, {tp, nonconst_int}));
        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator[]", FunctionType(type_ref, {tp, nonconst_int}));
    }

    return arrays[type][size];
}
