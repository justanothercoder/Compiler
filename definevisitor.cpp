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
#include "externnode.hpp"
#include "checkvisitor.hpp"
#include "modulesymbol.hpp"

void DefineVisitor::visit(ImportNode *node)
{
//    node -> scope -> define(new ModuleSymbol(node -> lib, node -> scope));
//    node -> root -> accept(*this);
}

void DefineVisitor::visit(ExternNode *node) 
{
    auto return_type = fromTypeInfo(node -> return_type_info, node -> scope);

    std::vector<const Type*> params_types;

    for ( auto i : node -> params )
    {
        auto param_type = fromTypeInfo(i.second, node -> scope);
        params_types.push_back(param_type);
    }

    auto type = TypeFactory::getFunctionType(return_type, std::move(FunctionTypeInfo(params_types)));

    node -> definedSymbol = new FunctionSymbol(node -> name
                                             , type
                                             , new FunctionScope("_" + node -> name
                                                               , node -> scope
                                                               , false
                                                               , false)
                                             , {false, false, false}
                                             );
    
    node -> definedSymbol -> is_unsafe = node -> is_unsafe;
    node -> scope -> define(node -> definedSymbol);
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
    auto fromTypeInfo = [&] (TypeInfo type_info) -> const Type*
    {
        if ( node -> traits.is_constructor && type_info.type_name == static_cast<StructSymbol*>(node -> scope) -> getName() )
        {
            const Type *type = static_cast<const StructSymbol*>(node -> scope);

            if ( type_info.is_ref )
                type = TypeFactory::getReference(type);

            if ( type_info.is_const )
                type = TypeFactory::getConst(type);

            return type;
        }
        return DefineVisitor::fromTypeInfo(type_info, node -> func_scope);
    };

    auto return_type = fromTypeInfo(node -> return_type_info);

    std::vector<const Type*> params_types;

    if ( node -> traits.is_method )
    {
        auto _this_type = TypeFactory::getReference(static_cast<StructSymbol*>(node -> scope));
        params_types.push_back(_this_type);

        auto _this_sym = new VariableSymbol("this", _this_type, VariableSymbolType::PARAM);

        node -> params_symbols.push_back(_this_sym);
        node -> func_scope -> define(_this_sym);
    }

    for ( auto i : node -> params )
    {
        auto param_type = fromTypeInfo(i.second);

        params_types.push_back(param_type);

        auto param_sym = new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM);

        node -> params_symbols.push_back(param_sym);
        node -> func_scope -> define(param_sym);
    }

    auto type = TypeFactory::getFunctionType(return_type, std::move(FunctionTypeInfo(params_types)));

    node -> definedSymbol = new FunctionSymbol(
                                           node -> traits.is_constructor ? static_cast<StructSymbol*>(node -> scope) -> getName() : node -> name
                                         , type
                                         , node -> func_scope
                                         , node -> traits
    );
    
    node -> func_scope -> func = node -> definedSymbol;
    node -> definedSymbol -> is_unsafe = node -> is_unsafe;
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
    CheckVisitor visitor;
    node -> expr -> accept(visitor);

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
void DefineVisitor::visit(ModuleNode* ) { }
void DefineVisitor::visit(TypeNode* ) { }
void DefineVisitor::visit(FunctionNode* ) { }
void DefineVisitor::visit(ModuleMemberAccessNode* ) { }
