#include "compiler.hpp"
#include "scope.hpp"
#include "structdeclarationnode.hpp"
#include "typefactory.hpp"
#include "exprnode.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "modulesymbol.hpp"
#include "compilableunit.hpp"
#include "templatedeclarationnode.hpp"
#include "comp.hpp"

VariableType Compiler::fromTypeInfo(const TypeInfo& type_info, Scope *scope)
{
    auto type_name = type_info.name();

    const Type *type;

    if ( type_info.moduleName() == "" )
        type = scope -> resolveType(type_name);
    else
    {
        auto module = Comp::getUnit(type_info.moduleName()) -> module_symbol;
        assert(module -> isModule());
        type = module -> resolveType(type_name);
    }

//    if ( type_info.pointer_depth > 0 && !scope -> isUnsafeBlock() )
//        throw SemanticError("Using pointer type in safe block " + scope -> getScopeName() + ".");
    
//    if ( type_info.array_dimensions.size() > 0 && !scope -> isUnsafeBlock() )
//        throw SemanticError("Using plain array type in safe block " + scope -> getScopeName() + ".");

    if ( type == nullptr )
        throw SemanticError(type_name + " is not a type.");
   
    for ( auto modifier : type_info.modifiers() )
    {
        if ( modifier.isPointer() )
            type = TypeFactory::getPointer(type);
        else      
        {
            auto dim = *modifier.dimension();

            if ( !dim -> isCompileTimeExpr() )
                throw SemanticError("Array dimension is not compile-time expression.");
            type = TypeFactory::getArray(type, *dim -> getCompileTimeValue());
        }
    }

    if ( type_info.isRef() )
        type = TypeFactory::getReference(type);

    assert(type != nullptr);
    return VariableType(type, type_info.isConst());
}

std::shared_ptr<DeclarationNode> Compiler::getSpecDecl(const TemplateSymbol *sym, TemplateArguments template_arguments)
{
    if ( auto decl = sym -> holder() -> getInstance(template_arguments) )
        return decl;

    auto decl = sym -> holder() -> instantiateWithArguments(template_arguments);
    sym -> holder() -> addInstance(template_arguments, decl);
    return decl;
}

const Symbol* Compiler::getSpec(const TemplateSymbol *sym, TemplateArguments tmpl_arguments)
{
    return getSpecDecl(sym, tmpl_arguments) -> getDefinedSymbol();
}
