#include "typefactory.hpp"

#include "pointertype.hpp"
#include "referencetype.hpp"
#include "arraytype.hpp"
#include "consttype.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"

#include "globaltable.hpp"

const Type* TypeFactory::getPointer(const Type *type)
{
    static std::map<const Type*, const Type*> pointers;

    if ( type -> getTypeKind() == TypeKind::REFERENCE )
        return nullptr;

    auto it = pointers.find(type);

    if ( it == std::end(pointers) )
    {
        pointers[type] = new PointerType(type);

        const auto& tp = pointers[type];
        auto tp_ref = getReference(tp);

        auto pointer_assign = new FunctionSymbol("operator=", 
                                                 tp_ref, 
                                                 {tp_ref, getConst(tp_ref)}, 
                                                 BuiltIns::global_scope, 
                                                 {false, false, true}
        );
        BuiltIns::global_scope -> define(pointer_assign);

        auto pointer_add = new FunctionSymbol("operator+", 
                                              tp, 
                                              {tp, BuiltIns::int_type}, 
                                              BuiltIns::global_scope, 
                                              {false, false, true}
        );
        BuiltIns::global_scope -> define(pointer_add);
    }

    return pointers[type];
}

const Type* TypeFactory::getReference(const Type *type)
{
    static std::map<const Type*, const Type*> references;

    if ( type -> getTypeKind() == TypeKind::REFERENCE )
        return nullptr;

    auto it = references.find(type);

    if ( it == std::end(references) )
        references[type] = new ReferenceType(type);

    return references[type];
}

const Type* TypeFactory::getConst(const Type *type)
{
    static std::map<const Type*, const Type*> consts;

    if ( type -> getTypeKind() == TypeKind::CONSTTYPE )
        return type;

    auto it = consts.find(type);

    if ( it == std::end(consts) )
        consts[type] = new ConstType(type);

    return consts[type];
}
    
const Type* TypeFactory::getArray(const Type *type, int size)
{
    static std::map<const Type*, std::map<int, const Type*> > arrays;

    if ( type -> getTypeKind() == TypeKind::REFERENCE )
        return nullptr;

    auto it = arrays.find(type);    

    if ( it == std::end(arrays) || it -> second.find(size) == std::end(it -> second) )
    {
        arrays[type][size] = new ArrayType(type, size);

        const auto& tp = arrays[type][size];
        auto tp_ref = getReference(tp);

        auto array_assign = new FunctionSymbol("operator=", 
                                               tp_ref, 
                                               {tp_ref, getConst(tp_ref)}, 
                                               BuiltIns::global_scope, 
                                               {false, false, true}
        );
        BuiltIns::global_scope -> define(array_assign);

        auto array_add = new FunctionSymbol("operator+", 
                                            getPointer(type), 
                                            {tp, BuiltIns::int_type}, 
                                            BuiltIns::global_scope, 
                                            {false, false, true}
        );
        BuiltIns::global_scope -> define(array_add);

        auto array_elem = new FunctionSymbol("operator[]", 
                                             getReference(type), 
                                             {tp, BuiltIns::int_type}, 
                                             BuiltIns::global_scope, 
                                             {false, false, true}
        );
        BuiltIns::global_scope -> define(array_elem);
    }

    return arrays[type][size];
}
