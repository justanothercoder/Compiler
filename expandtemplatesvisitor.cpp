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

    Logger::log("Expanding " + node -> toString());
    Logger::log("Scope: " + node -> scope -> getScopeName());

    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));    
}

void ExpandTemplatesVisitor::visit(NewExpressionNode* node)
{
    visitChildren(node);
    node -> typeInfo(preprocessTypeInfo(node -> typeInfo(), node -> scope.get()));
}
    
TemplateParam ExpandTemplatesVisitor::getTemplateParam(TemplateParamInfo info)
{
    struct ExtractTemplateParam : boost::static_visitor<TemplateParam>
    {
        auto operator()(const std::shared_ptr<ExprNode>& expr) { return TemplateParam(*expr -> getCompileTimeValue()); }
        auto operator()(const TypeInfo& type_info)             { return TemplateParam(type_info); }
    } extract;

    return boost::apply_visitor(extract, info);
}

TypeInfo ExpandTemplatesVisitor::preprocessTypeInfo(TypeInfo type_info, Scope* scope)
{
    Logger::log("Preprocessing " + type_info.toString());

    if ( type_info.moduleName() != "" )
        scope = Comp::getUnit(type_info.moduleName()) -> module_symbol.get();

    assert(scope != nullptr);

    auto type = scope -> resolve(type_info.name());

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

void ExpandTemplatesVisitor::visit(TemplateStructDeclarationNode* node)   { node -> scope -> define(node -> defined_symbol); }
void ExpandTemplatesVisitor::visit(TemplateFunctionDeclarationNode* node) { node -> scope -> define(node -> defined_symbol); }

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
void ExpandTemplatesVisitor::visit(TypeNode* ) { }
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
