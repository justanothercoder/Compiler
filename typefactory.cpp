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
    static std::map<const Type*, std::unique_ptr<const Type> > pointers;

    if ( type -> isReference() )
        return nullptr;

    auto it = pointers.find(type);

    if ( it == std::end(pointers) )
    {
        pointers[type] = std::make_unique<PointerType>(type);
        const auto& _tp = pointers[type].get();

        auto tp = VariableType(_tp, false);
        auto tp_ref = VariableType(getReference(_tp), false);
        auto const_tp_ref = VariableType(getReference(_tp), true);            
    
        auto nonconst_int = VariableType(BuiltIns::int_type.get(), false);

        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator=", FunctionType(tp_ref, {tp_ref, const_tp_ref}));
        static_cast<GlobalScope*>(BuiltIns::global_scope.get()) -> defineBuiltInOperator("operator+", FunctionType(tp, {tp, nonconst_int}));
    }

    return pointers[type].get();
}

const Type* TypeFactory::getReference(const Type *type)
{
    static std::map<const Type*, std::unique_ptr<const Type> > references;

    if ( type -> isReference() )
        return nullptr;

    auto it = references.find(type);

    if ( it == std::end(references) )
        references[type] = std::make_unique<ReferenceType>(type);

    return references[type].get();
}

const Type* TypeFactory::getArray(const Type *type, int size)
{
    static std::map<const Type*, std::map<int, std::unique_ptr<const Type> > > arrays;

    if ( type -> isReference() )
        return nullptr;

    auto it = arrays.find(type);

    if ( it == std::end(arrays) || it -> second.find(size) == std::end(it -> second) )
    {
        arrays[type][size] = std::make_unique<ArrayType>(type, size);

        const auto& _tp = arrays[type][size].get();

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

    return arrays[type][size].get();
}
