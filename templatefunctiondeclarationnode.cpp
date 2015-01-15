#include "templatefunctiondeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatefunctionsymbol.hpp"

#include "logger.hpp"

TemplateFunctionDeclarationNode::TemplateFunctionDeclarationNode(const std::string& name
                                                               , FunctionDeclarationInfo info
                                                               , ASTNode statements
                                                               , FunctionTraits traits
                                                               , bool is_unsafe
                                                               , TemplateParamsList template_params)
    : name(name)
    , info_(info)
    , statements(std::move(statements))
    , traits(traits)
    , is_unsafe(is_unsafe)
    , template_params(template_params)
{
    defined_symbol = std::make_shared<TemplateFunctionSymbol>(name, template_params, this);
}

void TemplateFunctionDeclarationNode::build_scope() { }
void TemplateFunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ASTNode TemplateFunctionDeclarationNode::copyTree() const 
{
    return std::make_unique<TemplateFunctionDeclarationNode>(name, info_, statements -> copyTree(), traits, is_unsafe, template_params); 
}

std::string TemplateFunctionDeclarationNode::toString() const
{
    auto res = info_.returnTypeInfo().toString() + " " + name + "(";

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

std::shared_ptr<DeclarationNode> TemplateFunctionDeclarationNode::instantiateWithTemplateInfo(TemplateInfo template_info) 
{
    auto templates_name = std::string("");
    templates_name += name + "~";
    for ( auto param : template_info.template_params )
    {
        if ( param.which() == 0 )
            templates_name += boost::get<TypeInfo>(param).name();
        else
            templates_name += std::to_string(boost::get<int>(param));
    }

    auto decl = std::make_shared<FunctionDeclarationNode>(templates_name, info_, statements -> copyTree(), traits, is_unsafe);

	decl -> scope = scope;
    decl -> template_info = template_info;
    decl -> build_scope();

    return decl;
}

std::vector< std::shared_ptr<DeclarationNode> > TemplateFunctionDeclarationNode::allInstances() const 
{
    std::vector< std::shared_ptr<DeclarationNode> > result;

    for ( auto instance : instances )
        result.emplace_back(instance.second);

    return result;
}

unsigned long long TemplateFunctionDeclarationNode::hashTemplateParams(std::vector<TemplateParam> template_params) const
{
    unsigned long long P = 31, pow = 1, ans = 0;

    for ( size_t i = 0; i < template_params.size(); ++i )
    {
        if ( template_params[i].which() == 0 )
            ans += static_cast<int>(std::hash<std::string>()(boost::get<TypeInfo>(template_params[i]).name()) * pow);
        else
            ans += static_cast<int>(boost::get<int>(template_params[i])) * pow;

        pow *= P;
    }

    return ans;
}
    
const FunctionDeclarationInfo& TemplateFunctionDeclarationNode::info() const { return info_; }
