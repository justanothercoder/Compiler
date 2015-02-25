#include "expandtemplatesvisitor.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "dotnode.hpp"
#include "callnode.hpp"
#include "addrnode.hpp"
#include "whilenode.hpp"
#include "unarynode.hpp"
#include "importnode.hpp"
#include "returnnode.hpp"
#include "bracketnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "templatefunctionnode.hpp"
#include "structdeclarationnode.hpp"
#include "templatedeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "modulesymbol.hpp"
#include "typesymbol.hpp"
#include "templatestructsymbol.hpp"
#include "templatefunctionsymbol.hpp"
#include "compilableunit.hpp"
#include "comp.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "genssavisitor.hpp"
#include "logger.hpp"
    
void ExpandTemplatesVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void ExpandTemplatesVisitor::visit(FunctionDeclarationNode* node)
{
    node -> info().returnTypeInfo() = preprocessTypeInfo(node -> info().returnTypeInfo(), node -> functionScope());

    for ( auto& param : node -> info().formalParams() )
        param.second = preprocessTypeInfo(param.second, node -> functionScope());

    visitChildren(node);
}

void ExpandTemplatesVisitor::visit(VariableDeclarationNode* node) 
{
    visitChildren(node);
    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));    
}

void ExpandTemplatesVisitor::visit(NewExpressionNode* node)
{
    visitChildren(node);
    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));
}
    
TypeInfo ExpandTemplatesVisitor::preprocessTypeInfo(TypeInfo type_info, const Scope* scope)
{
    if ( type_info.moduleName() != "" )
        scope = Comp::getUnit(type_info.moduleName()) -> module_symbol.get();

    assert(scope != nullptr);

    auto type = scope -> resolve(type_info.name());

    if ( type == nullptr )
        throw SemanticError(type_info.name() + " is not a type");
    
    if ( auto tmpl = dynamic_cast<const TemplateSymbol*>(type) )
    {
        auto decl = instantiateSpec(tmpl, type_info.templateArgumentsInfo());
        type_info.name(decl -> getDefinedSymbol() -> getName());
    
        if ( type_info.moduleName() != "" )
        {
            DefineVisitor define_visitor;
            CheckVisitor check_visitor;
            GenSSAVisitor gen_visitor(Comp::code);

            for ( auto visitor : std::vector<ASTVisitor*>{this, &define_visitor, &check_visitor, &gen_visitor} )
                decl -> accept(*visitor);
        }
    }

    return type_info;
}

std::shared_ptr<DeclarationNode> ExpandTemplatesVisitor::instantiateSpec(const TemplateSymbol* tmpl
                                                                       , const TemplateArgumentsInfo& template_arguments_info)
{   
    assert(template_arguments_info.size() == tmpl -> templateParams().size());

    auto tmpl_arguments = TemplateArguments{ };
    for ( const auto& argument_info : template_arguments_info )
       tmpl_arguments.push_back(getTemplateArgument(argument_info)); 
    
    if ( auto inst = tmpl -> holder() -> getInstance(tmpl_arguments) )
        return inst;

    auto decl = getSpecDecl(tmpl, tmpl_arguments);

    tmpl -> holder() -> addInstance(tmpl_arguments, decl);

    decl -> accept(*this);
    return decl;
}

void ExpandTemplatesVisitor::visit(TemplateStructDeclarationNode* node)   
{
    auto sym = std::unique_ptr<TemplateSymbol>(node -> defined_symbol);
    node -> scope -> define(std::move(sym)); 
}

void ExpandTemplatesVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    auto sym = std::unique_ptr<TemplateSymbol>(node -> defined_symbol);
    node -> scope -> define(std::move(sym)); 
}

void ExpandTemplatesVisitor::visit(IfNode*                node) { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(ForNode*               node) { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(DotNode*               node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(AddrNode*              node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(CallNode*              node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(UnaryNode*             node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(WhileNode*             node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(ReturnNode*            node) { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(BracketNode*           node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(StatementNode*         node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(UnsafeBlockNode*       node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(BinaryOperatorNode*    node) { visitChildren(node); }
void ExpandTemplatesVisitor::visit(StructDeclarationNode* node) { visitChildren(node); }

void ExpandTemplatesVisitor::visit(VarInferTypeDeclarationNode* node) { node -> expr() -> accept(*this); }

void ExpandTemplatesVisitor::visit(NullNode* ) { }
void ExpandTemplatesVisitor::visit(BreakNode* ) { } 
void ExpandTemplatesVisitor::visit(StringNode* ) { }
void ExpandTemplatesVisitor::visit(NumberNode* ) { }
void ExpandTemplatesVisitor::visit(ExternNode* ) { }
void ExpandTemplatesVisitor::visit(ImportNode* ) { } 
void ExpandTemplatesVisitor::visit(ModuleNode* ) { }
void ExpandTemplatesVisitor::visit(VariableNode* ) { }
void ExpandTemplatesVisitor::visit(FunctionNode* ) { }
void ExpandTemplatesVisitor::visit(TemplateFunctionNode* ) { } 
void ExpandTemplatesVisitor::visit(ModuleMemberAccessNode* ) { } 
