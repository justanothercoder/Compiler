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
  
//    auto func_scope = new FunctionScope("_" + node -> name(), node -> scope.get(), false);
    auto symbol = factory.makeFunction(node -> name(), type, FunctionTraits::simple(), node -> isUnsafe());

    node -> setDefinedSymbol(symbol.get());
    node -> scope -> define(std::move(symbol));
}

void DefineVisitor::visit(FunctionDeclarationNode* node)
{
    bool is_in_struct = (declarations_stack.back() -> isType());
    auto struc = is_in_struct ? static_cast<const TypeSymbol*>(declarations_stack.back()) : nullptr;
        
    auto fromTypeInfo = [&] (auto&& type_info) 
    {
        if ( node -> traits().is_constructor )
        {
            assert(is_in_struct);
            assert(type_info.name() == struc -> getName());

            const Type* type = struc;

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
        assert(is_in_struct);

        auto _this_type = TypeFactory::getReference(struc);
        params_types.push_back(_this_type);

        auto this_param = factory.makeVariable("this", _this_type, VariableSymbolType::PARAM);
        node -> addParamSymbol(std::move(this_param));
    }

    for ( auto param : node -> info().formalParams() )
    {
        auto param_type = fromTypeInfo(param.second);
        params_types.push_back(param_type);

        auto param_sym = factory.makeVariable(param.first, param_type, VariableSymbolType::PARAM);
        node -> addParamSymbol(std::move(param_sym));
    }

    auto type = FunctionType(return_type, FunctionTypeInfo(params_types));

    auto function_name = node -> traits().is_constructor ? struc -> getName() : node -> name();
    auto function = factory.makeFunction(function_name, type, node -> traits(), node -> isUnsafe(), node -> functionScope(), node);

    node -> setDefinedSymbol(function.get());
    node -> scope -> define(std::move(function));

    node -> body() -> accept(*this);
}

void DefineVisitor::visit(VariableDeclarationNode* node)
{
    auto var_type = fromTypeInfo(node -> typeInfo(), node -> scope.get());

    if ( var_type == BuiltIns::void_type )
        throw SemanticError("can't declare a variable of 'void' type.");

    auto var_symbol_type = (node -> isField() ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE);
    auto var = factory.makeVariable(node -> name(), var_type, var_symbol_type);
    
    node -> setDefinedSymbol(var.get());
    node -> scope -> define(std::move(var));
}

void DefineVisitor::visit(VarInferTypeDeclarationNode* node)
{
    CheckVisitor visitor;
    node -> expr() -> accept(visitor);

    if ( node -> expr() -> getType() == BuiltIns::void_type )
        throw SemanticError("can't define variable of 'void' type");

    auto var = factory.makeVariable(node -> name(), node -> expr() -> getType());

    node -> setDefinedSymbol(var.get());
    node -> scope -> define(std::move(var));
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

void DefineVisitor::visit(StructDeclarationNode *node) 
{
    SymbolFactory factory;
    auto struc = factory.makeStruct(node -> name(), node -> structScope());

    declarations_stack.push_back(struc.get());
    node -> scope -> define(std::move(struc));
    visitChildren(node); 

    declarations_stack.pop_back();
}

void DefineVisitor::visit(IfNode *node)                { visitChildren(node); }
void DefineVisitor::visit(ForNode *node)               { visitChildren(node); } 
void DefineVisitor::visit(WhileNode *node)             { visitChildren(node); } 
void DefineVisitor::visit(StatementNode* node)         { visitChildren(node); }

void DefineVisitor::visit(ReturnNode* node)      { node -> expr() -> accept(*this); }
void DefineVisitor::visit(UnsafeBlockNode* node) { node -> block() -> accept(*this); }

void DefineVisitor::visit(DotNode* ) { }
void DefineVisitor::visit(CallNode* ) { } 
void DefineVisitor::visit(AddrNode* ) { }
void DefineVisitor::visit(NullNode* ) { }
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
