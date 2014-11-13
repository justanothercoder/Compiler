#include "isdefinedvisitor.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "addrnode.hpp"
#include "dotnode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "numbernode.hpp"
#include "callnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "importnode.hpp"
#include "variablesymbol.hpp"
#include "structsymbol.hpp"
#include "functionsymbol.hpp"

void IsDefinedVisitor::visit(ImportNode *node) 
{
    node -> root -> accept(*this);
}

void IsDefinedVisitor::visit(IfNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(ForNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(WhileNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(BracketNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(UnaryNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(NewExpressionNode *node) 
{
    if ( fromTypeInfo(node -> type_info, node -> scope) == nullptr )
        throw SemanticError("No such type " + node -> type_info.type_name);

    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(BinaryOperatorNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(StructDeclarationNode *node) 
{
    for ( auto decl : node -> inner )
        static_cast<DeclarationNode*>(decl) -> getDefinedSymbol() -> is_defined = true;

    for ( auto decl : node -> inner )
        decl -> accept(*this);    
}

void IsDefinedVisitor::visit(FunctionDeclarationNode *node)
{
    node -> definedSymbol -> is_defined = true;

    for ( auto param : node -> params_symbols )
        param -> is_defined = true;
}

void IsDefinedVisitor::visit(VariableDeclarationNode *node) 
{
    node -> getDefinedSymbol() -> is_defined = true;
}

void IsDefinedVisitor::visit(AddrNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(DotNode *node) 
{
    node -> base -> accept(*this);
}

void IsDefinedVisitor::visit(StatementNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(VariableNode *node) 
{
    auto sym = node -> scope -> resolve(node -> name);

    if ( sym == nullptr || !sym -> is_defined )
        throw SemanticError("No such symbol '" + node -> name + "'.");
}

void IsDefinedVisitor::visit(CallNode *node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void IsDefinedVisitor::visit(ReturnNode *node) 
{
    node -> expr -> accept(*this);
}

void IsDefinedVisitor::visit(UnsafeBlockNode *node) 
{
    node -> block -> accept(*this);
}

void IsDefinedVisitor::visit(VarInferTypeDeclarationNode *node) 
{
    node -> getDefinedSymbol() -> is_defined = true;
}

void IsDefinedVisitor::visit(TemplateStructDeclarationNode *) { }
void IsDefinedVisitor::visit(NullNode *) { }
void IsDefinedVisitor::visit(StringNode *) { }
void IsDefinedVisitor::visit(NumberNode *) { }

