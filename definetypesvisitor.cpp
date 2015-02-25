#include "definetypesvisitor.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "whilenode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "structdeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatestructsymbol.hpp"
#include "logger.hpp"

void DefineTypesVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void DefineTypesVisitor::visit(TemplateStructDeclarationNode* node)
{
    auto sym = std::unique_ptr<TemplateSymbol>(node -> defined_symbol);
    node -> scope -> define(std::move(sym)); 
}
    
void DefineTypesVisitor::visit(StructDeclarationNode *node) 
{
    Logger::log("defining " + node -> toString());

    auto struc = factory.makeStruct(node -> name(), node -> structScope());

    node -> setDefinedSymbol(struc.get());

    node -> scope -> define(std::move(struc));
    visitChildren(node); 
}

void DefineTypesVisitor::visit(IfNode *node)                { visitChildren(node); }
void DefineTypesVisitor::visit(ForNode *node)               { visitChildren(node); } 
void DefineTypesVisitor::visit(WhileNode *node)             { visitChildren(node); } 
void DefineTypesVisitor::visit(StatementNode* node)         { visitChildren(node); }

void DefineTypesVisitor::visit(UnsafeBlockNode* node) { node -> block() -> accept(*this); }

void DefineTypesVisitor::visit(DotNode* ) { }
void DefineTypesVisitor::visit(CallNode* ) { } 
void DefineTypesVisitor::visit(AddrNode* ) { }
void DefineTypesVisitor::visit(NullNode* ) { }
void DefineTypesVisitor::visit(UnaryNode* ) { }
void DefineTypesVisitor::visit(BreakNode* ) { } 
void DefineTypesVisitor::visit(StringNode* ) { }
void DefineTypesVisitor::visit(NumberNode* ) { }
void DefineTypesVisitor::visit(ModuleNode* ) { }
void DefineTypesVisitor::visit(ImportNode* ) { }
void DefineTypesVisitor::visit(BracketNode* ) { }
void DefineTypesVisitor::visit(VariableNode* ) { }
void DefineTypesVisitor::visit(FunctionNode* ) { }
void DefineTypesVisitor::visit(NewExpressionNode* ) { }
void DefineTypesVisitor::visit(BinaryOperatorNode* ) { }
void DefineTypesVisitor::visit(TemplateFunctionNode* ) { }
void DefineTypesVisitor::visit(ModuleMemberAccessNode* ) { }
void DefineTypesVisitor::visit(ExternNode* ) { }
void DefineTypesVisitor::visit(ReturnNode* ) { }
void DefineTypesVisitor::visit(FunctionDeclarationNode* ) { } 
void DefineTypesVisitor::visit(VariableDeclarationNode* ) { } 
void DefineTypesVisitor::visit(VarInferTypeDeclarationNode* ) { } 
void DefineTypesVisitor::visit(TemplateFunctionDeclarationNode* ) { }

