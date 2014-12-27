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
    auto type_name = type_info.type_name;

    const Type *type;

    if ( type_info.module_name == "" )
        type = scope -> resolveType(type_name);
    else
    {
        auto module = Comp::getUnit(type_info.module_name) -> module_symbol;
        assert(module -> getSymbolType() == SymbolType::MODULE);
        type = static_cast<ModuleSymbol*>(module) -> resolveType(type_name);
    }

//    if ( type_info.pointer_depth > 0 && !scope -> isUnsafeBlock() )
//        throw SemanticError("Using pointer type in safe block " + scope -> getScopeName() + ".");
    
//    if ( type_info.array_dimensions.size() > 0 && !scope -> isUnsafeBlock() )
//        throw SemanticError("Using plain array type in safe block " + scope -> getScopeName() + ".");

    if ( type == nullptr )
        throw SemanticError(type_name + " is not a type.");
    
    for ( int i = 0; i < type_info.pointer_depth; ++i )
        type = TypeFactory::getPointer(type);

    for ( auto dim : type_info.array_dimensions )
    {
        if ( !dim -> isCompileTimeExpr() )
            throw SemanticError("Array dimension is not compile-time expression.");
        type = TypeFactory::getArray(type, *dim -> getCompileTimeValue());
    }
    
    if ( type_info.is_ref )
        type = TypeFactory::getReference(type);

    assert(type != nullptr);
    return VariableType(type, type_info.is_const);
}

DeclarationNode* Compiler::getSpecDecl(const TemplateSymbol *sym, std::vector<TemplateParam> template_params)
{
    if ( auto decl = sym -> holder() -> getInstance(template_params) )
        return decl;

    auto decl = sym -> holder() -> instantiateWithTemplateInfo(TemplateInfo(sym, template_params));
    sym -> holder() -> addInstance(template_params, decl);
    return decl;
}

const Symbol* Compiler::getSpec(const TemplateSymbol *sym, std::vector<TemplateParam> tmpl_params)
{
    return getSpecDecl(sym, tmpl_params) -> getDefinedSymbol();
}
