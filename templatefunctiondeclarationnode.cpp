#include "templatefunctiondeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "templatefunctionsymbol.hpp"

#include "logger.hpp"

TemplateFunctionDeclarationNode::TemplateFunctionDeclarationNode(std::string name
                                                               , FunctionDeclarationInfo info
                                                               , AST* statements
                                                               , FunctionTraits traits
                                                               , bool is_unsafe
                                                               , TemplateParamsList template_params)
    : name(name)
    , info(info)
    , statements(statements)
    , traits(traits)
    , is_unsafe(is_unsafe)
    , template_params(template_params)
{
    defined_symbol = new TemplateFunctionSymbol(name, template_params, this);
}

void TemplateFunctionDeclarationNode::build_scope() { }
void TemplateFunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

AST* TemplateFunctionDeclarationNode::copyTree() const 
{
    return new TemplateFunctionDeclarationNode(name, info, statements -> copyTree(), traits, is_unsafe, template_params); 
}

std::string TemplateFunctionDeclarationNode::toString() const
{
    std::string res = info.returnTypeInfo().toString() + " " + name + "(";

    if ( !info.formalParams().empty() )
    {
        const auto& params = info.formalParams();

        auto it = std::begin(params);

        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ")";
    res += statements -> toString();

    return res;
}

Symbol* TemplateFunctionDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

void TemplateFunctionDeclarationNode::addInstance(std::vector<TemplateParam> template_params, DeclarationNode* decl) 
{
    instances[hashTemplateParams(template_params)] = decl;
}

DeclarationNode* TemplateFunctionDeclarationNode::getInstance(std::vector<TemplateParam> template_params) const 
{
	auto it = instances.find(hashTemplateParams(template_params));
    return it != std::end(instances) ? it -> second : nullptr;
}

DeclarationNode* TemplateFunctionDeclarationNode::instantiateWithTemplateInfo(TemplateInfo template_info) 
{
    auto templates_name = std::string("");
    templates_name += name + "~";
    for ( auto param : template_info.template_params )
    {
        if ( param.which() == 0 )
            templates_name += boost::get<TypeInfo>(param).type_name;
        else
            templates_name += std::to_string(boost::get<int>(param));
    }

    Logger::log("Templated name = '" + templates_name + "'");

    auto decl = new FunctionDeclarationNode(templates_name, info, statements -> copyTree(), traits, is_unsafe);

	decl -> scope = scope;
    decl -> template_info = template_info;
    decl -> build_scope();

    return decl;
}

std::vector<DeclarationNode*> TemplateFunctionDeclarationNode::allInstances() const 
{
    std::vector<DeclarationNode*> result;

    for ( auto instance : instances )
        result.push_back(instance.second);

    return result;
}

unsigned long long TemplateFunctionDeclarationNode::hashTemplateParams(std::vector<TemplateParam> template_params) const
{
    unsigned long long P = 31, pow = 1, ans = 0;

    for ( size_t i = 0; i < template_params.size(); ++i )
    {
        if ( template_params[i].which() == 0 )
            ans += static_cast<int>(std::hash<std::string>()(boost::get<TypeInfo>(template_params[i]).type_name) * pow);
        else
            ans += static_cast<int>(boost::get<int>(template_params[i])) * pow;

        pow *= P;
    }

    return ans;
}
