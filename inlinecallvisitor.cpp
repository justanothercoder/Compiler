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
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "localscope.hpp"
#include "callablenode.hpp"

#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "builtins.hpp"
#include "markreturnasinlinevisitor.hpp"

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

bool InlineCallVisitor::shouldBeInlined(const FunctionSymbol* function)
{
    if ( function -> function_decl == nullptr )
        return false;

    auto params = function -> type().typeInfo().params(); 
    return std::all_of(std::begin(params), std::end(params), [](auto&& t)
    {
        return t.isReference()
            || t.base() -> isPointer()
            || t.base() == BuiltIns::int_type.get()
            || t.base() == BuiltIns::char_type.get();
    });
};

InlineInfo InlineCallVisitor::inlineCall(const FunctionSymbol* function)
{
    auto function_decl = function -> function_decl;
    
    auto function_body = function_decl -> getChildren()[0] -> copyTree();
    auto local_scope = std::make_shared<LocalScope>(function_decl -> scope.get());

    function_body -> scope = local_scope;
    function_body -> build_scope();
  
    std::vector< std::shared_ptr<VariableSymbol> > locals;

    for ( auto param : function_decl -> paramsSymbols() ) 
    {
        auto new_var = std::make_shared<VariableSymbol>(param -> getName(), param -> getType());
        locals.push_back(new_var);
        local_scope -> define(new_var);
    }

    local_scope -> define(std::make_shared<VariableSymbol>("$", function -> type().returnType()));

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
void InlineCallVisitor::visit(TypeNode* ) { }
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
