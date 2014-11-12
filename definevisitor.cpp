#include "definevisitor.hpp"
#include "callhelper.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "structsymbol.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "functionsymbol.hpp"
#include "variabledeclarationnode.hpp"
#include "addrnode.hpp"
#include "dotnode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "callnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "importnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "nullnode.hpp"
#include "filehelper.hpp"
#include "typefactory.hpp"
#include "globaltable.hpp"
#include "builtins.hpp"

void DefineVisitor::visit(ImportNode *node)
{
    node -> root -> accept(*this);
}

void DefineVisitor::visit(IfNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DefineVisitor::visit(ForNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DefineVisitor::visit(WhileNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DefineVisitor::visit(StructDeclarationNode *node)
{
    for ( auto decl : node -> inner )
        decl -> accept(*this);
}

void DefineVisitor::visit(FunctionDeclarationNode *node)
{
    const auto& template_info = node -> scope -> getTemplateInfo();

    auto fromTypeInfo = [&] (TypeInfo type_info) -> const Type*
    {
        if ( template_info.sym != nullptr && type_info.type_name == template_info.sym -> getName() )
            type_info.type_name = static_cast<StructSymbol*>(node -> scope) -> getName();

        if ( node -> definedSymbol -> isMethod() && type_info.type_name == static_cast<StructSymbol*>(node -> scope) -> getName() )
        {
            const Type *type = static_cast<const StructSymbol*>(node -> scope);

            if ( type_info.is_ref )
                type = TypeFactory::getReference(type);

            if ( type_info.is_const )
                type = TypeFactory::getConst(type);

            return type;
        }
        return DefineVisitor::fromTypeInfo(type_info, node -> definedSymbol);
    };

    auto return_type = fromTypeInfo(node -> return_type_info);

    std::vector<const Type*> params_types;

    if ( node -> traits.is_method )
    {
        auto _this_type = TypeFactory::getReference(static_cast<StructSymbol*>(node -> scope));
        params_types.push_back(_this_type);

        auto _this_sym = new VariableSymbol("this", _this_type, VariableSymbolType::PARAM);

        node -> params_symbols.push_back(_this_sym);
        node -> definedSymbol -> define(_this_sym);
    }

    for ( auto i : node -> params )
    {
        auto param_type = fromTypeInfo(i.second);

        params_types.push_back(param_type);

        auto param_sym = new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM);

        node -> params_symbols.push_back(param_sym);
        node -> definedSymbol -> define(param_sym);
    }

    FunctionTypeInfo function_type_info(params_types);

    node -> definedSymbol -> return_type = return_type;
    node -> definedSymbol -> function_type_info = function_type_info;

    node -> scope -> define(node -> definedSymbol);

    node -> statements -> accept(*this);
}

void DefineVisitor::visit(VariableDeclarationNode *node)
{
    auto var_type = fromTypeInfo(node -> type_info, node -> scope);

    if ( var_type == BuiltIns::void_type )
        throw SemanticError("can't declare a variable of 'void' type.");

    node -> definedSymbol = new VariableSymbol(node -> name, 
                                               var_type, 
                                               (node -> is_field ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE)
    );

    node -> scope -> define(node -> definedSymbol);
}

void DefineVisitor::visit(StatementNode *node)
{
    for ( auto i : node -> statements )
        i -> accept(*this);
}

void DefineVisitor::visit(ReturnNode *node)
{
    node -> expr -> accept(*this);
}

void DefineVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void DefineVisitor::visit(VarInferTypeDeclarationNode *node)
{
//	node -> expr -> check();

    if ( node -> scope -> resolve(node -> name) != nullptr )
        throw SemanticError(node -> name + " is already defined");

    if ( node -> expr -> getType() == BuiltIns::void_type )
        throw SemanticError("can't define variable of 'void' type");

    node -> definedSymbol = new VariableSymbol(node -> name, node -> expr -> getType());

    node -> scope -> define(node -> definedSymbol);
}

void DefineVisitor::visit(TemplateStructDeclarationNode *node)
{
    for ( auto instance : node -> instances )
        instance -> accept(*this);
}

void DefineVisitor::visit(BracketNode *) { }
void DefineVisitor::visit(UnaryNode *) { }
void DefineVisitor::visit(NewExpressionNode *) { }
void DefineVisitor::visit(BinaryOperatorNode *) { }
void DefineVisitor::visit(AddrNode *) { }
void DefineVisitor::visit(NullNode *) { }
void DefineVisitor::visit(DotNode *) { }
void DefineVisitor::visit(VariableNode *) { }
void DefineVisitor::visit(StringNode *) { }
void DefineVisitor::visit(NumberNode *) { }
void DefineVisitor::visit(CallNode *) { }