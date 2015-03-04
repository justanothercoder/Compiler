#include "inlinecallvisitor.hpp"
#include "callnode.hpp"
#include "ifnode.hpp"
#include "dotnode.hpp"
#include "fornode.hpp"
#include "addrnode.hpp"
#include "whilenode.hpp"
#include "unarynode.hpp"
#include "returnnode.hpp"
#include "bracketnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "localscope.hpp"
#include "callablenode.hpp"

#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "builtins.hpp"
#include "markreturnasinlinevisitor.hpp"
#include "symbolfactory.hpp"

#include "logger.hpp"

void InlineCallVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visitCallable(CallableNode* node)
{
    auto function = node -> callInfo().callee;
    
    if ( !shouldBeInlined(function) )
        return;
    
    node -> inlineInfo(inlineCall(function));
}

bool InlineCallVisitor::shouldBeInlined(const FunctionalSymbol* function)
{
    if ( function -> getFunctionBody() == nullptr || function -> innerScope() == nullptr )
        return false;

    auto params = function -> type().typeInfo().params(); 
    return std::all_of(std::begin(params), std::end(params), [](auto&& t)
    {
        return t.isReference() || t.base() -> isPointer() || isIntType(t.base()) || isCharType(t.base());
    });
};

InlineInfo InlineCallVisitor::inlineCall(const FunctionalSymbol* function)
{
    SymbolFactory factory;

    auto function_body = function -> getFunctionBody() -> copyTree();
    //    auto local_scope = std::make_shared<LocalScope>(function_decl -> scope.get());
    auto local_scope = std::make_shared<LocalScope>(function -> innerScope());

    function_body -> scope = local_scope;
    function_body -> build_scope();
  
    std::vector<const VarSymbol*> locals;
/*
    for ( auto param : function_decl -> paramsSymbols() ) 
    {
        auto new_var = factory.makeVariable(param -> getName(), param -> typeOf());
        locals.push_back(new_var.get());
        local_scope -> define(std::move(new_var));
    }
*/
    
    for ( auto param : function_body -> scope -> getVars() ) 
    {
        if ( param -> isParam() )
        {
            auto new_var = factory.makeVariable(param -> getName(), param -> typeOf());
            locals.push_back(new_var.get());
            local_scope -> define(std::move(new_var));
        }
    }
    
    local_scope -> define(factory.makeVariable("$", function -> type().returnType()));

    MarkReturnAsInlineVisitor mark(function);

    ExpandTemplatesVisitor expand;
    DefineVisitor define;
    CheckVisitor check;

    InlineCallVisitor inline_call;

    for ( auto visitor : std::vector<ASTVisitor*>{&mark, &expand, &define, &check, &inline_call} )
        function_body -> accept(*visitor);

    return InlineInfo(std::move(function_body), locals);
}

void InlineCallVisitor::visit(CallNode* node)           { visitChildren(node); visitCallable(node); } 
void InlineCallVisitor::visit(UnaryNode* node)          { visitChildren(node); visitCallable(node); } 
void InlineCallVisitor::visit(BracketNode* node)        { visitChildren(node); visitCallable(node); }
void InlineCallVisitor::visit(BinaryOperatorNode* node) { visitChildren(node); visitCallable(node); }

void InlineCallVisitor::visit(NewExpressionNode* node) 
{
    visitChildren(node); 

    auto function = node -> callInfo().callee;
    
    if ( !shouldBeInlined(function) )
        return;
    
    node -> inlineInfo(inlineCall(function));
}

void InlineCallVisitor::visit(VariableDeclarationNode* node) 
{
    visitChildren(node);

    if ( !node -> isField() )
    {
        auto function = node -> callInfo().callee;

        if ( !function || !shouldBeInlined(function) )
            return;

        node -> inlineInfo(inlineCall(function));
    }
}

void InlineCallVisitor::visit(VarInferTypeDeclarationNode* node) 
{
    visitChildren(node); 

    auto function = node -> callInfo().callee;

    if ( !function || !shouldBeInlined(function) )
        return;

    node -> inlineInfo(inlineCall(function));
}

void InlineCallVisitor::visit(TemplateStructDeclarationNode* node) 
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}
    
void InlineCallVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}

void InlineCallVisitor::visit(LambdaNode*) { }

void InlineCallVisitor::visit(IfNode* node)                      { visitChildren(node); }
void InlineCallVisitor::visit(DotNode* node)                     { visitChildren(node); } 
void InlineCallVisitor::visit(ForNode* node)                     { visitChildren(node); }
void InlineCallVisitor::visit(AddrNode* node)                    { visitChildren(node); }
void InlineCallVisitor::visit(WhileNode* node)                   { visitChildren(node); }
void InlineCallVisitor::visit(ReturnNode* node)                  { visitChildren(node); }
void InlineCallVisitor::visit(StatementNode* node)               { visitChildren(node); }
void InlineCallVisitor::visit(UnsafeBlockNode* node)             { visitChildren(node); }
void InlineCallVisitor::visit(StructDeclarationNode* node)       { visitChildren(node); }
void InlineCallVisitor::visit(FunctionDeclarationNode* node)     { visitChildren(node); }

void InlineCallVisitor::visit(NullNode* ) { }
void InlineCallVisitor::visit(BreakNode* ) { }
void InlineCallVisitor::visit(ImportNode* ) { }
void InlineCallVisitor::visit(StringNode* ) { }
void InlineCallVisitor::visit(NumberNode* ) { }
void InlineCallVisitor::visit(ModuleNode* ) { }
void InlineCallVisitor::visit(ExternNode* ) { }
void InlineCallVisitor::visit(VariableNode* ) { }
void InlineCallVisitor::visit(FunctionNode* ) { }
void InlineCallVisitor::visit(TemplateFunctionNode* ) { }
void InlineCallVisitor::visit(ModuleMemberAccessNode* ) { }
