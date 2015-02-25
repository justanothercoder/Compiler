#include "templateargument.hpp"

unsigned long long hashTemplateArguments(const TemplateArguments& template_arguments)
{
    unsigned long long P = 31, pow = 1, ans = 0;

    for ( size_t i = 0; i < template_arguments.size(); ++i )
    {
        if ( template_arguments[i].which() == 0 )
            ans += static_cast<int>(std::hash<std::string>()(boost::get<TypeInfo>(template_arguments[i]).name()) * pow);
        else
            ans += static_cast<int>(boost::get<int>(template_arguments[i])) * pow;

        pow *= P;
    }

    return ans;
}
    
TemplateArgument getTemplateArgument(TemplateArgumentInfo info)
{
    struct ExtractTemplateParam : boost::static_visitor<TemplateArgument>
    {
        auto operator()(const std::shared_ptr<ExprNode>& expr) { return TemplateArgument(*expr -> getCompileTimeValue()); }
        auto operator()(const TypeInfo& type_info)             { return TemplateArgument(type_info); }
    } extract;

    return boost::apply_visitor(extract, info);
}

std::string toString(const TemplateArgumentsInfo& template_arguments_info)
{
    auto result = std::string("");
    result += "<";
    
    struct StringifyVisitor : boost::static_visitor<std::string>
    {
        auto operator()(const std::shared_ptr<ExprNode>& expr) { return expr -> toString(); }
        auto operator()(const TypeInfo& type_info) { return type_info.toString(); }
    } stringify;

    auto it = std::begin(template_arguments_info);

    result += boost::apply_visitor(stringify, *it);
    for ( ++it; it != std::end(template_arguments_info); ++it )
        result += ", " + boost::apply_visitor(stringify, *it);

    result += ">";
    return result;
}

TemplateArguments getTemplateArguments(TemplateArgumentsInfo info)
{
    auto template_arguments = TemplateArguments{ };
    
    for ( const auto& i : info )
        template_arguments.emplace_back(getTemplateArgument(i));

    return template_arguments;
}
