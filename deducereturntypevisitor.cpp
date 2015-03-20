#include "deducereturntypevisitor.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "dotnode.hpp"
#include "addrnode.hpp"
#include "callnode.hpp"
#include "whilenode.hpp"
#include "unarynode.hpp"
#include "lambdanode.hpp"
#include "bracketnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "returnnode.hpp"
#include "checkvisitor.hpp"
#include "builtins.hpp"
    
void DeduceReturnTypeVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DeduceReturnTypeVisitor::visit(IfNode* node)             { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(ForNode* node)            { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(DotNode* node)            { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(AddrNode* node)           { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(CallNode* node)           { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(WhileNode* node)          { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(UnaryNode* node)          { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(LambdaNode* node)         { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(BracketNode* node)        { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(StatementNode* node)      { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(UnsafeBlockNode* node)    { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(NewExpressionNode* node)  { visitChildren(node); }
void DeduceReturnTypeVisitor::visit(BinaryOperatorNode* node) { visitChildren(node); }

void DeduceReturnTypeVisitor::visit(NullNode* )               { }
void DeduceReturnTypeVisitor::visit(StringNode* )             { }
void DeduceReturnTypeVisitor::visit(NumberNode* )             { }
void DeduceReturnTypeVisitor::visit(BreakNode* )              { }
void DeduceReturnTypeVisitor::visit(ExternNode* )             { }
void DeduceReturnTypeVisitor::visit(ModuleNode* )             { }
void DeduceReturnTypeVisitor::visit(ImportNode* )             { }
void DeduceReturnTypeVisitor::visit(FunctionNode* )           { }
void DeduceReturnTypeVisitor::visit(VariableNode* )           { }
void DeduceReturnTypeVisitor::visit(TemplateFunctionNode* )   { }

void DeduceReturnTypeVisitor::visit(StructDeclarationNode* )           { }
void DeduceReturnTypeVisitor::visit(FunctionDeclarationNode* )         { }
void DeduceReturnTypeVisitor::visit(VariableDeclarationNode* )         { }
void DeduceReturnTypeVisitor::visit(VarInferTypeDeclarationNode* )     { }
void DeduceReturnTypeVisitor::visit(TemplateStructDeclarationNode* )   { }
void DeduceReturnTypeVisitor::visit(TemplateFunctionDeclarationNode* ) { }

void DeduceReturnTypeVisitor::visit(ReturnNode* node) 
{
    CheckVisitor check;
    node -> expr() -> accept(check);

    auto type = node -> expr() -> getType();
    
    if ( current_type == boost::none )
        current_type = type;
    else
    {
        if ( !current_type -> base() -> isConvertableTo(type.base()) )
            throw SemanticError("Can't deduce appropriate return type: can't convert '" + current_type -> getName() + "' to '" + type.getName() + "'");
    }
}
    
VariableType DeduceReturnTypeVisitor::deduceReturnType(AST* node)
{
    node -> accept(*this);
    return current_type == boost::none ? BuiltIns::void_type : *current_type;
}
