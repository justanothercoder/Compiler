#include "templatefunctiondeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatefunctionsymbol.hpp"
#include "rebuildtreevisitor.hpp"
#include "logger.hpp"

TemplateFunctionDeclarationNode::TemplateFunctionDeclarationNode(const std::string& name
                                                               , FunctionDeclarationInfo info
                                                               , ASTNode statements
                                                               , FunctionTraits traits
                                                               , bool is_unsafe
                                                               , TemplateParamsList template_params)
    : name_(name)
    , info_(info)
    , statements(std::move(statements))
    , traits_(traits)
    , is_unsafe(is_unsafe)
    , template_params(template_params)
{
    defined_symbol = std::make_shared<TemplateFunctionSymbol>(name, template_params, this);
}

void TemplateFunctionDeclarationNode::build_scope() { }
void TemplateFunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ASTNode TemplateFunctionDeclarationNode::copyTree() const 
{
    return std::make_unique<TemplateFunctionDeclarationNode>(name_, info_, statements -> copyTree(), traits_, is_unsafe, template_params); 
}

std::string TemplateFunctionDeclarationNode::toString() const
{
    auto res = std::string("");    
    res += "template <";

    if ( !template_params.empty() )
    {
        auto it = std::begin(template_params);
        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(template_params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ">\n";
    res += info_.returnTypeInfo().toString() + " " + name_ + "(";

    if ( !info_.formalParams().empty() )
    {
        const auto& params = info_.formalParams();

        auto it = std::begin(params);
        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ")";
    res += statements -> toString();

    return res;
}

const Symbol* TemplateFunctionDeclarationNode::getDefinedSymbol() const { return defined_symbol.get(); }

void TemplateFunctionDeclarationNode::addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> decl) 
{
    instances[hashTemplateParams(template_params)] = decl;
}

std::shared_ptr<DeclarationNode> TemplateFunctionDeclarationNode::getInstance(std::vector<TemplateParam> template_params) const 
{
	auto it = instances.find(hashTemplateParams(template_params));
    return it != std::end(instances) ? it -> second : nullptr;
}

std::shared_ptr<DeclarationNode> TemplateFunctionDeclarationNode::instantiateWithParams(std::vector<TemplateParam> params) 
{
    auto template_info = TemplateInfo(defined_symbol.get(), params);
    RebuildTreeVisitor rebuild(template_info);

    statements -> accept(rebuild);    
    auto stat = rebuild.result();

    auto new_return_type_info = rebuild.processTypeInfo(info_.returnTypeInfo());
    auto new_formal_params = std::vector<ParamInfo>{ };

    for ( auto param : info_.formalParams() )
        new_formal_params.emplace_back(param.first, rebuild.processTypeInfo(param.second));
    auto new_info = FunctionDeclarationInfo(new_return_type_info, new_formal_params);

    auto decl = std::make_shared<FunctionDeclarationNode>(template_info.getInstName(), new_info, std::move(stat), traits_, is_unsafe);

	decl -> scope = scope;
    decl -> build_scope();

    return decl;
}

std::vector<DeclarationNode*> TemplateFunctionDeclarationNode::allInstances() const 
{
    auto result = std::vector<DeclarationNode*>{ };

    for ( auto instance : instances )
        result.emplace_back(instance.second.get());

    return result;
}

std::string TemplateFunctionDeclarationNode::name() const { return name_; }
const FunctionDeclarationInfo& TemplateFunctionDeclarationNode::info() const { return info_; }   

FunctionTraits TemplateFunctionDeclarationNode::traits() const { return traits_; }
bool TemplateFunctionDeclarationNode::isUnsafe() const { return is_unsafe; }

const TemplateParamsList& TemplateFunctionDeclarationNode::templateParams() const { return template_params; }
AST* TemplateFunctionDeclarationNode::body() { return statements.get(); }
