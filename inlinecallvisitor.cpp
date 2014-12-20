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
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "localscope.hpp"

#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "builtins.hpp"
#include "markreturnasinlinevisitor.hpp"

#include "logger.hpp"

void InlineCallVisitor::visit(CallNode* node)
{
    for ( auto param : node -> params )
        param -> accept(*this);

    auto shouldBeInlined = [](const FunctionSymbol* function)
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

    auto function = node -> call_info.callee;
    
    if ( !shouldBeInlined(function) )
        return;
    
    auto function_decl = function -> function_decl;
    
    auto function_body = function_decl -> getChildren()[0] -> copyTree();
    auto local_scope = new LocalScope(node -> scope);

    function_body -> scope = local_scope;
    function_body -> build_scope();
    
    Logger::log("Inlining function " + function -> getName() + " {");
    Logger::log(function_body -> toString());
    Logger::log("}");

    for ( auto param : function_decl -> params_symbols ) 
    {
        Logger::log("Defining in local scope variable " + param -> getName());
        auto new_var = new VariableSymbol(param -> getName(), param -> getType());
        Logger::log("Defining in local scope variable " + param -> getName());
        node -> inline_locals.push_back(new_var);
        Logger::log("Defining in local scope variable " + param -> getName());
        local_scope -> define(new_var);
    }

//    MarkReturnAsInlineVisitor mark;

    ExpandTemplatesVisitor expand;
    DefineVisitor define;
    CheckVisitor check;

    for ( auto visitor : std::vector<ASTVisitor*>{/*&mark, */&expand, &define, &check} )
        function_body -> accept(*visitor);

    node -> inline_call_body = function_body;
}

void InlineCallVisitor::visit(IfNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(DotNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(ForNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(AddrNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(WhileNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(UnaryNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(ReturnNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(BracketNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(StatementNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(UnsafeBlockNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(BinaryOperatorNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(StructDeclarationNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(FunctionDeclarationNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(VariableDeclarationNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(VarInferTypeDeclarationNode* node) 
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void InlineCallVisitor::visit(TemplateStructDeclarationNode* node) 
{
    for ( auto instance : node -> instances )
        instance -> accept(*this);
}

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
