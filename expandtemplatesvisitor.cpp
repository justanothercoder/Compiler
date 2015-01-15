#include "expandtemplatesvisitor.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "whilenode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "statementnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "templatestructsymbol.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "newexpressionnode.hpp"
#include "callnode.hpp"
#include "dotnode.hpp"
#include "importnode.hpp"
#include "templatefunctionnode.hpp"
#include "modulesymbol.hpp"
#include "compilableunit.hpp"
#include "comp.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "genssavisitor.hpp"
#include "templatedeclarationnode.hpp"
#include "templatefunctionsymbol.hpp"
#include "logger.hpp"
    
void ExpandTemplatesVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void ExpandTemplatesVisitor::visit(FunctionDeclarationNode *node)
{
    const auto& template_info = node -> scope -> templateInfo();

    if ( template_info.sym && node -> info().returnTypeInfo().name() == template_info.sym -> getName() )
        node -> info().returnTypeInfo().name(static_cast<StructSymbol*>(node -> scope.get()) -> getName());

    node -> info().returnTypeInfo() = preprocessTypeInfo(node -> info().returnTypeInfo(), node -> functionScope());

    for ( auto& param : node -> info().formalParams() )
    {
        if ( template_info.sym && param.second.name() == template_info.sym -> getName() )
            param.second.name(static_cast<StructSymbol*>(node -> scope.get()) -> getName());

        param.second = preprocessTypeInfo(param.second, node -> functionScope());
    }

    visitChildren(node);
}

void ExpandTemplatesVisitor::visit(VariableDeclarationNode *node) 
{
    for ( const auto& modifier : node -> typeInfo().modifiers() )
    {
        if ( modifier.isDimension() )
        {
            auto dim = *modifier.dimension();
            dim -> scope = node -> scope;
        }
    }   

    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));
}

void ExpandTemplatesVisitor::visit(NewExpressionNode *node)
{
    for ( const auto& modifier : node -> typeInfo().modifiers() )
    {
        if ( modifier.isDimension() )
        {
            auto dim = *modifier.dimension();
            dim -> scope = node -> scope;
        }
    }   

    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));
}
    
TemplateParam ExpandTemplatesVisitor::getTemplateParam(TemplateParamInfo info)
{
    if ( info.which() == 1 )
        return TemplateParam(boost::get<TypeInfo>(info));
    else
        return TemplateParam(*boost::get<ExprNode*>(info) -> getCompileTimeValue());
}

TypeInfo ExpandTemplatesVisitor::preprocessTypeInfo(TypeInfo type_info, Scope *scope)
{
    const auto& template_info = scope -> templateInfo();

    if ( template_info.sym && template_info.isIn(type_info.name()) )
    {
        auto replace = template_info.getReplacement(type_info.name());
        auto temp_info = boost::get<TypeInfo>(*replace);

        auto modifiers = temp_info.modifiers();
        modifiers.insert(std::end(modifiers), std::begin(type_info.modifiers()), std::end(type_info.modifiers()));

        type_info = TypeInfo(temp_info.name()
                           , type_info.isRef()
                           , type_info.isConst()
                           , temp_info.templateParams()
                           , modifiers
                           , type_info.moduleName());
    }
   
    Scope* sc;
    if ( type_info.moduleName() == "" )
        sc = scope;
    else
        sc = Comp::getUnit(type_info.moduleName()) -> module_symbol.get();
    assert(sc != nullptr);

    auto type = sc -> resolve(type_info.name());

    if ( type == nullptr )
        throw SemanticError(type_info.name() + " is not a type");
    
    if ( auto tmpl = dynamic_cast<const TemplateSymbol*>(type) )
    {
        auto decl = instantiateSpec(tmpl, type_info.templateParams());
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

std::shared_ptr<DeclarationNode> ExpandTemplatesVisitor::instantiateSpec(const TemplateSymbol* tmpl, std::vector<TemplateParamInfo> template_params)
{   
    assert(template_params.size() == tmpl -> templateSymbols().size());

    std::vector<TemplateParam> tmpl_params;
    for ( auto param_info : template_params )
       tmpl_params.push_back(getTemplateParam(param_info)); 
    
    if ( auto inst = tmpl -> holder() -> getInstance(tmpl_params) )
        return inst;

    auto decl = getSpecDecl(tmpl, tmpl_params);

    tmpl -> holder() -> addInstance(tmpl_params, decl);

    decl -> accept(*this);
    return decl;
}

void ExpandTemplatesVisitor::visit(VarInferTypeDeclarationNode* node) { node -> expr() -> accept(*this); }
void ExpandTemplatesVisitor::visit(DotNode* node) { node -> base() -> accept(*this); }

void ExpandTemplatesVisitor::visit(TemplateStructDeclarationNode* node)
{ 
    node -> scope -> define(std::shared_ptr<const Symbol>(node -> getDefinedSymbol())); 
}

void ExpandTemplatesVisitor::visit(TemplateFunctionDeclarationNode* node) 
{ 
    node -> scope -> define(std::shared_ptr<const Symbol>(node -> getDefinedSymbol())); 
}

void ExpandTemplatesVisitor::visit(CallNode* node)              { visitChildren(node); }
void ExpandTemplatesVisitor::visit(UnsafeBlockNode* node)       { visitChildren(node); }
void ExpandTemplatesVisitor::visit(StatementNode* node)         { visitChildren(node); }
void ExpandTemplatesVisitor::visit(ReturnNode* node)            { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(IfNode* node)                { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(ForNode* node)               { visitChildren(node); } 
void ExpandTemplatesVisitor::visit(WhileNode* node)             { visitChildren(node); }
void ExpandTemplatesVisitor::visit(StructDeclarationNode* node) { visitChildren(node); }

void ExpandTemplatesVisitor::visit(AddrNode* ) { }
void ExpandTemplatesVisitor::visit(NullNode* ) { }
void ExpandTemplatesVisitor::visit(TypeNode* ) { }
void ExpandTemplatesVisitor::visit(BreakNode* ) { } 
void ExpandTemplatesVisitor::visit(UnaryNode* ) { }
void ExpandTemplatesVisitor::visit(StringNode* ) { }
void ExpandTemplatesVisitor::visit(NumberNode* ) { }
void ExpandTemplatesVisitor::visit(ExternNode* ) { }
void ExpandTemplatesVisitor::visit(ImportNode* ) { } 
void ExpandTemplatesVisitor::visit(ModuleNode* ) { }
void ExpandTemplatesVisitor::visit(BracketNode* ) { }
void ExpandTemplatesVisitor::visit(VariableNode* ) { }
void ExpandTemplatesVisitor::visit(FunctionNode* ) { }
void ExpandTemplatesVisitor::visit(BinaryOperatorNode* ) { }
void ExpandTemplatesVisitor::visit(TemplateFunctionNode* ) { } 
void ExpandTemplatesVisitor::visit(ModuleMemberAccessNode* ) { } 
