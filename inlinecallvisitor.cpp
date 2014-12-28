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

bool InlineCallVisitor::shouldBeInlined(const FunctionSymbol* function)
{
    if ( function -> function_decl == nullptr )
        return false;

    auto params = function -> type().typeInfo().params_types; 
    return std::all_of(std::begin(params), std::end(params), [](VariableType t)
    {
        return t.isReference()
            || t.base() -> getTypeKind() == TypeKind::POINTER
            || t.base() == BuiltIns::int_type
            || t.base() == BuiltIns::char_type;
    });
};

InlineInfo InlineCallVisitor::inlineCall(const FunctionSymbol* function)
{
    auto function_decl = function -> function_decl;
    
    auto function_body = function_decl -> getChildren()[0] -> copyTree();
    auto local_scope = new LocalScope(function_decl -> scope);

    function_body -> scope = local_scope;
    function_body -> build_scope();
  
    std::vector<VariableSymbol*> locals;

    for ( auto param : function_decl -> params_symbols ) 
    {
        auto new_var = new VariableSymbol(param -> getName(), param -> getType());
        locals.push_back(new_var);
        local_scope -> define(new_var);
    }
    local_scope -> define(new VariableSymbol("$", function -> type().returnType()));

    MarkReturnAsInlineVisitor mark;

    ExpandTemplatesVisitor expand;
    DefineVisitor define;
    CheckVisitor check;

    InlineCallVisitor inline_call;

    for ( auto visitor : std::vector<ASTVisitor*>{&mark, &expand, &define, &check, &inline_call} )
        function_body -> accept(*visitor);

    return InlineInfo(function_body, locals);
}

void InlineCallVisitor::visit(CallNode* node)
{
    for ( auto param : node -> params )
        param -> accept(*this);

    auto function = node -> call_info.callee;
    
    if ( !shouldBeInlined(function) )
        return;
    
    node -> inline_info = inlineCall(function);
}

void InlineCallVisitor::visit(VariableDeclarationNode* node) 
{
    visitChildren(node);

    if ( !node -> is_field )
    {
        auto function = node -> call_info.callee;

        if ( !function || !shouldBeInlined(function) )
            return;

        node -> inline_info = inlineCall(function);
    }
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
void InlineCallVisitor::visit(UnaryNode* node)                   { visitChildren(node); }
void InlineCallVisitor::visit(ReturnNode* node)                  { visitChildren(node); }
void InlineCallVisitor::visit(BracketNode* node)                 { visitChildren(node); }
void InlineCallVisitor::visit(StatementNode* node)               { visitChildren(node); }
void InlineCallVisitor::visit(UnsafeBlockNode* node)             { visitChildren(node); }
void InlineCallVisitor::visit(BinaryOperatorNode* node)          { visitChildren(node); }
void InlineCallVisitor::visit(StructDeclarationNode* node)       { visitChildren(node); }
void InlineCallVisitor::visit(FunctionDeclarationNode* node)     { visitChildren(node); }
void InlineCallVisitor::visit(VarInferTypeDeclarationNode* node) { visitChildren(node); }

void InlineCallVisitor::visit(NullNode* ) { }
void InlineCallVisitor::visit(TypeNode* ) { }
void InlineCallVisitor::visit(BreakNode* ) { }
void InlineCallVisitor::visit(ImportNode* ) { }
void InlineCallVisitor::visit(StringNode* ) { }
void InlineCallVisitor::visit(NumberNode* ) { }
void InlineCallVisitor::visit(ModuleNode* ) { }
void InlineCallVisitor::visit(VariableNode* ) { }
void InlineCallVisitor::visit(FunctionNode* ) { }
void InlineCallVisitor::visit(ModuleMemberAccessNode* ) { }
void InlineCallVisitor::visit(NewExpressionNode* ) { }
void InlineCallVisitor::visit(ExternNode* ) { }
void InlineCallVisitor::visit(TemplateFunctionNode* ) { }
