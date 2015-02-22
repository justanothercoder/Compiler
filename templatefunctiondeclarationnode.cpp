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
                                                               , TemplateParamsInfo template_params_info)
    : name_(name)
    , info_(info)
    , statements(std::move(statements))
    , traits_(traits)
    , is_unsafe(is_unsafe)
    , template_params_info(template_params_info)
{
    defined_symbol = new TemplateFunctionSymbol(name, template_params_info, this);
}

void TemplateFunctionDeclarationNode::build_scope() { }
void TemplateFunctionDeclarationNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

ASTNode TemplateFunctionDeclarationNode::copyTree() const 
{
    return std::make_unique<TemplateFunctionDeclarationNode>(name_, info_, statements -> copyTree(), traits_, is_unsafe, template_params_info); 
}

std::string TemplateFunctionDeclarationNode::toString() const
{
    auto res = std::string("");    
    res += "template <";

    if ( !template_params_info.empty() )
    {
        auto it = std::begin(template_params_info);
        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(template_params_info); ++it )
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

Symbol* TemplateFunctionDeclarationNode::getDefinedSymbol() const { return defined_symbol; }

void TemplateFunctionDeclarationNode::addInstance(TemplateArguments template_arguments, std::shared_ptr<DeclarationNode> decl) 
{
    instances[hashTemplateArguments(template_arguments)] = decl;
}

std::shared_ptr<DeclarationNode> TemplateFunctionDeclarationNode::getInstance(TemplateArguments template_arguments) const 
{
	auto it = instances.find(hashTemplateArguments(template_arguments));
    return it != std::end(instances) ? it -> second : nullptr;
}

std::shared_ptr<DeclarationNode> TemplateFunctionDeclarationNode::instantiateWithArguments(TemplateArguments arguments) 
{
    auto template_info = TemplateInfo(defined_symbol, arguments);
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

const TemplateParamsInfo& TemplateFunctionDeclarationNode::templateParamsInfo() const { return template_params_info; }
AST* TemplateFunctionDeclarationNode::body() { return statements.get(); }
