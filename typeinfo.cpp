#include "typeinfo.hpp"
#include "exprnode.hpp"

TypeInfo::TypeInfo() { }

TypeInfo::TypeInfo(std::string               type_name
                 , bool                      is_ref
                 , bool                      is_const
                 , TemplateArgumentsInfo     template_arguments
                 , std::vector<TypeModifier> modifiers
                 , std::string               module_name) : type_name         (type_name)
                                                          , is_ref            (is_ref)
                                                          , is_const          (is_const)
                                                          , template_arguments(template_arguments)
                                                          , modifiers_        (modifiers)
                                                          , module_name       (module_name)
{

}

std::string TypeInfo::toString() const
{
    auto res = type_name;

    if ( !module_name.empty() )
        res = module_name + "." + res;

    if ( is_const )
        res = "const " + res;

    if ( !template_arguments.empty() )
    {
        res += "<";

        auto it = std::begin(template_arguments);

        struct StringifyVisitor : boost::static_visitor<std::string>
        {
            auto operator()(const std::shared_ptr<ExprNode>& expr) { return expr -> toString(); }
            auto operator()(const TypeInfo& type_info) { return type_info.toString(); }
        } stringify;

        res += boost::apply_visitor(stringify, *it);        

        for ( ++it; it != std::end(template_arguments); ++it )
            res += ", " + boost::apply_visitor(stringify, *it);

        res += ">";
    }

    for ( auto modifier : modifiers_ )
        res += modifier.toString();

    if ( is_ref )
        res += "&";

    return res;
}

const std::string& TypeInfo::name() const { return type_name; }
void TypeInfo::name(const std::string& name) { type_name = name; }

bool TypeInfo::isRef() const { return is_ref; }
bool TypeInfo::isConst() const { return is_const; }

const std::vector<TypeModifier>& TypeInfo::modifiers() const { return modifiers_; }
const TemplateArgumentsInfo& TypeInfo::templateArgumentsInfo() const { return template_arguments; }
const std::string& TypeInfo::moduleName() const { return module_name; }

bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return lhs.type_name          == rhs.type_name
        && lhs.is_ref             == rhs.is_ref
        && lhs.is_const           == rhs.is_const
        && lhs.template_arguments == rhs.template_arguments
        && lhs.modifiers_         == rhs.modifiers_
        && lhs.module_name        == rhs.module_name;
}

bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) { return !(lhs == rhs); }    
