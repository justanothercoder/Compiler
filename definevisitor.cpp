#include "definevisitor.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "whilenode.hpp"
#include "returnnode.hpp"
#include "externnode.hpp"
#include "importnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "typefactory.hpp"
#include "globaltable.hpp"
#include "builtins.hpp"
#include "checkvisitor.hpp"
#include "structsymbol.hpp"
#include "modulesymbol.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "logger.hpp"

void DefineVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DefineVisitor::visit(ExternNode* node) 
{
    auto return_type = fromTypeInfo(node -> info().returnTypeInfo(), node -> scope.get());

    auto params_types = std::vector<VariableType>{ };

    for ( auto formal_param : node -> info().formalParams() )
    {
        auto param_type = fromTypeInfo(formal_param.second, node -> scope.get());
        params_types.push_back(std::move(param_type));
    }

    auto type = FunctionType(std::move(return_type), std::move(FunctionTypeInfo(params_types)));

    auto symbol = std::make_shared<FunctionSymbol>(node -> name(), type
                                                 , new FunctionScope("_" + node -> name(), node -> scope.get(), false)
                                                 , FunctionTraits::simple());
    symbol -> is_unsafe = node -> isUnsafe();

    node -> setDefinedSymbol(symbol);
    node -> scope -> define(symbol);
}

void DefineVisitor::visit(FunctionDeclarationNode* node)
{
    auto fromTypeInfo = [&] (auto&& type_info) 
    {
        if ( node -> traits().is_constructor && type_info.name() == static_cast<StructSymbol*>(node -> scope.get()) -> getName() )
        {
            const Type* type = static_cast<const StructSymbol*>(node -> scope.get());

            if ( type_info.isRef() )
                type = TypeFactory::getReference(type);

            return VariableType(type, type_info.isConst());
        }
        return DefineVisitor::fromTypeInfo(type_info, node -> functionScope());
    };

    auto return_type = fromTypeInfo(node -> info().returnTypeInfo());

    auto params_types = std::vector<VariableType>{ };

    if ( node -> traits().is_method )
    {
        auto _this_type = TypeFactory::getReference(static_cast<StructSymbol*>(node -> scope.get()));
        params_types.push_back(_this_type);

        node -> addParamSymbol(std::make_shared<VariableSymbol>("this", _this_type, VariableSymbolType::PARAM));
    }

    for ( auto param : node -> info().formalParams() )
    {
        auto param_type = fromTypeInfo(param.second);
        params_types.push_back(param_type);

        node -> addParamSymbol(std::make_shared<VariableSymbol>(param.first, param_type, VariableSymbolType::PARAM));
    }

    auto type = FunctionType(return_type, FunctionTypeInfo(params_types));

    auto function_name = node -> traits().is_constructor ? static_cast<StructSymbol*>(node -> scope.get()) -> getName() : node -> name();

    auto symbol = std::make_shared<FunctionSymbol>(function_name, type, node -> functionScope(), node -> traits());
    symbol -> function_decl = node;
    symbol -> is_unsafe = node -> isUnsafe();

    node -> setDefinedSymbol(symbol);
    
    node -> functionScope() -> func = symbol.get();
    node -> scope -> define(symbol);

    node -> body() -> accept(*this);
}

void DefineVisitor::visit(VariableDeclarationNode* node)
{
    auto var_type = fromTypeInfo(node -> typeInfo(), node -> scope.get());

    if ( var_type == BuiltIns::void_type.get() )
        throw SemanticError("can't declare a variable of 'void' type.");

    auto var_symbol_type = (node -> isField() ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE);

    auto symbol = std::make_shared<const VariableSymbol>(node -> name(), var_type, var_symbol_type);
    node -> setDefinedSymbol(symbol);
    node -> scope -> define(symbol);
}

void DefineVisitor::visit(VarInferTypeDeclarationNode* node)
{
    CheckVisitor visitor;
    node -> expr() -> accept(visitor);

    if ( node -> expr() -> getType() == BuiltIns::void_type.get() )
        throw SemanticError("can't define variable of 'void' type");

    auto symbol = std::make_shared<const VariableSymbol>(node -> name(), node -> expr() -> getType());

    node -> setDefinedSymbol(symbol);
    node -> scope -> define(symbol);
}

void DefineVisitor::visit(TemplateStructDeclarationNode* node)
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}
    
void DefineVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}

void DefineVisitor::visit(IfNode *node)                { visitChildren(node); }
void DefineVisitor::visit(ForNode *node)               { visitChildren(node); } 
void DefineVisitor::visit(WhileNode *node)             { visitChildren(node); } 
void DefineVisitor::visit(StructDeclarationNode *node) { visitChildren(node); } 
void DefineVisitor::visit(StatementNode* node)         { visitChildren(node); }

void DefineVisitor::visit(ReturnNode* node)      { node -> expr() -> accept(*this); }
void DefineVisitor::visit(UnsafeBlockNode* node) { node -> block() -> accept(*this); }

void DefineVisitor::visit(DotNode* ) { }
void DefineVisitor::visit(CallNode* ) { } 
void DefineVisitor::visit(AddrNode* ) { }
void DefineVisitor::visit(NullNode* ) { }
void DefineVisitor::visit(TypeNode* ) { }
void DefineVisitor::visit(UnaryNode* ) { }
void DefineVisitor::visit(BreakNode* ) { } 
void DefineVisitor::visit(StringNode* ) { }
void DefineVisitor::visit(NumberNode* ) { }
void DefineVisitor::visit(ModuleNode* ) { }
void DefineVisitor::visit(ImportNode* ) { }
void DefineVisitor::visit(BracketNode* ) { }
void DefineVisitor::visit(VariableNode* ) { }
void DefineVisitor::visit(FunctionNode* ) { }
void DefineVisitor::visit(NewExpressionNode* ) { }
void DefineVisitor::visit(BinaryOperatorNode* ) { }
void DefineVisitor::visit(TemplateFunctionNode* ) { }
void DefineVisitor::visit(ModuleMemberAccessNode* ) { }
