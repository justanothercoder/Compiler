#include "templatestructdeclarationnode.hpp"
#include "symboldefine.hpp"
#include "templateinfo.hpp"
#include "templatestructsymbol.hpp"
#include "structdeclarationnode.hpp"
#include "rebuildtreevisitor.hpp"

#include "logger.hpp"

TemplateStructDeclarationNode::TemplateStructDeclarationNode(std::string name
                                                           , std::vector<ASTNode> inner
                                                           , TemplateParamsList template_params) : name_(name)
                                                                                                 , inner(std::move(inner))
                                                                                                 , template_params(template_params)
{
    defined_symbol = std::make_shared<TemplateStructSymbol>(name, template_params, this);
}

void TemplateStructDeclarationNode::build_scope() { } 
void TemplateStructDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

Symbol* TemplateStructDeclarationNode::getDefinedSymbol() const { return defined_symbol.get(); }

void TemplateStructDeclarationNode::addInstance(std::vector<TemplateParam> template_params, std::shared_ptr<DeclarationNode> node)
{
    instances[hashTemplateParams(template_params)] = node;
}

std::shared_ptr<DeclarationNode> TemplateStructDeclarationNode::getInstance(std::vector<TemplateParam> template_params) const
{
	auto it = instances.find(hashTemplateParams(template_params));
    return it != std::end(instances) ? it -> second : nullptr;
}

std::vector<DeclarationNode*> TemplateStructDeclarationNode::allInstances() const
{
    auto result = std::vector<DeclarationNode*>{ };

    for ( auto instance : instances )
        result.emplace_back(instance.second.get());

    return result;
}

ASTNode TemplateStructDeclarationNode::copyTree() const
{
    auto inner_copy = std::vector<ASTNode>{ };
    for ( const auto& child : inner )
        inner_copy.push_back(child -> copyTree());

    return std::make_unique<TemplateStructDeclarationNode>(name_, std::move(inner_copy), template_params);
}

std::string TemplateStructDeclarationNode::toString() const
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

    res += "struct " + name_ + "\n{\n";

    for ( const auto& decl : inner )
        res += decl -> toString() + '\n';

    res += "}";
    return res;
}

std::shared_ptr<DeclarationNode> TemplateStructDeclarationNode::instantiateWithParams(std::vector<TemplateParam> params)
{
    auto template_info = TemplateInfo(defined_symbol.get(), params);
    RebuildTreeVisitor rebuild(template_info);

	auto vec = std::vector<ASTNode>{ };
	for ( const auto& decl : inner )
    {
        decl -> accept(rebuild);
		vec.push_back(rebuild.result());
    }

    auto decl = std::make_shared<StructDeclarationNode>(template_info.getInstName(), std::move(vec));

    Logger::log("Scope of template decl: " + scope -> getScopeName());
	
    decl -> scope = scope;
    decl -> build_scope();
    
    return decl;
}
   
std::string TemplateStructDeclarationNode::name() const { return name_; }
const TemplateParamsList& TemplateStructDeclarationNode::templateParams() const { return template_params; }
