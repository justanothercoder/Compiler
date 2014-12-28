#include "templatestructdeclarationnode.hpp"
#include "symboldefine.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(std::string name
                                                           , std::vector<AST*> inner
                                                           , std::vector< std::pair<std::string, TypeInfo> > template_params)
    : name(name)
    , inner(inner)
    , template_params(template_params)
{
    defined_symbol = new TemplateStructSymbol(name, template_params, this);
}

void TemplateStructDeclarationNode::build_scope() { } 
void TemplateStructDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

Symbol* TemplateStructDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

unsigned long long TemplateStructDeclarationNode::hashTemplateParams(std::vector<TemplateParam> template_params) const
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

void TemplateStructDeclarationNode::addInstance(std::vector<TemplateParam> template_params, DeclarationNode* node)
{
    instances[hashTemplateParams(template_params)] = node;
}

DeclarationNode* TemplateStructDeclarationNode::getInstance(std::vector<TemplateParam> template_params) const
{
	auto it = instances.find(hashTemplateParams(template_params));
    return it != std::end(instances) ? it -> second : nullptr;
}

std::vector<DeclarationNode*> TemplateStructDeclarationNode::allInstances() const
{
    std::vector<DeclarationNode*> result;

    for ( auto instance : instances )
        result.push_back(instance.second);

    return result;
}

AST* TemplateStructDeclarationNode::copyTree() const
{
    std::vector<AST*> inner_copy;
    for ( auto child : inner )
        inner_copy.push_back(child -> copyTree());

    return new TemplateStructDeclarationNode(name, inner_copy, template_params);
}

std::string TemplateStructDeclarationNode::toString() const
{
    std::string res = "struct " + name + "\n{\n";

    for ( auto decl : inner )
        res += decl -> toString() + '\n';

    res += "}";
    return res;
}

DeclarationNode* TemplateStructDeclarationNode::instantiateWithTemplateInfo(TemplateInfo info)
{
	std::vector<AST*> vec;

	for ( auto t : inner )
		vec.push_back(t -> copyTree());

    auto templates_name = std::string("");
    templates_name += name + "~";
    for ( auto param : info.template_params )
    {
        if ( param.which() == 0 )
            templates_name += boost::get<TypeInfo>(param).type_name;
        else
            templates_name += std::to_string(boost::get<int>(param));
    }

    auto decl = new StructDeclarationNode(templates_name, vec);

	decl -> scope = scope;
    decl -> template_info = info;
    decl -> build_scope();

    return decl;
}
