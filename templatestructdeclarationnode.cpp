#include "templatestructdeclarationnode.hpp"
#include "symboldefine.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(std::string name
                                                           , std::vector<ASTNode> inner
                                                           , TemplateParamsList template_params) : name(name)
                                                                                                 , inner(std::move(inner))
                                                                                                 , template_params(template_params)
{
    defined_symbol = std::make_shared<TemplateStructSymbol>(name, template_params, this);
}

void TemplateStructDeclarationNode::build_scope() { } 
void TemplateStructDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

Symbol* TemplateStructDeclarationNode::getDefinedSymbol() const { return defined_symbol.get(); }

unsigned long long TemplateStructDeclarationNode::hashTemplateParams(std::vector<TemplateParam> template_params) const
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

void TemplateStructDeclarationNode::addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> node)
{
    instances[hashTemplateParams(template_params)] = node;
}

std::shared_ptr<DeclarationNode> TemplateStructDeclarationNode::getInstance(std::vector<TemplateParam> template_params) const
{
	auto it = instances.find(hashTemplateParams(template_params));
    return it != std::end(instances) ? it -> second : nullptr;
}

std::vector< std::shared_ptr<DeclarationNode> > TemplateStructDeclarationNode::allInstances() const
{
    auto result = std::vector< std::shared_ptr<DeclarationNode> >{ };

    for ( auto instance : instances )
        result.emplace_back(instance.second);

    return result;
}

ASTNode TemplateStructDeclarationNode::copyTree() const
{
    auto inner_copy = std::vector<ASTNode>{ };
    for ( const auto& child : inner )
        inner_copy.push_back(child -> copyTree());

    return std::make_unique<TemplateStructDeclarationNode>(name, std::move(inner_copy), template_params);
}

std::string TemplateStructDeclarationNode::toString() const
{
    auto res = "struct " + name + "\n{\n";

    for ( const auto& decl : inner )
        res += decl -> toString() + '\n';

    res += "}";
    return res;
}

std::shared_ptr<DeclarationNode> TemplateStructDeclarationNode::instantiateWithTemplateInfo(TemplateInfo info)
{
	auto vec = std::vector<ASTNode>{ };

	for ( const auto& decl : inner )
		vec.push_back(decl -> copyTree());

    auto templates_name = std::string("");
    templates_name += name + "~";
    for ( auto param : info.template_params )
    {
        if ( param.which() == 0 )
            templates_name += boost::get<TypeInfo>(param).name();
        else
            templates_name += std::to_string(boost::get<int>(param));
    }

    auto decl = std::make_shared<StructDeclarationNode>(templates_name, std::move(vec));

	decl -> scope = scope;
    decl -> template_info = info;
    decl -> build_scope();

    return decl;
}
