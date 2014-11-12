#include "typeinfo.hpp"
#include "exprnode.hpp"

TypeInfo::TypeInfo()
{

}

TypeInfo::TypeInfo(std::string                    type_name
                 , bool                           is_ref
                 , bool                           is_const
                 , std::vector<TemplateParamInfo> template_params
                 , int                            pointer_depth
                 , std::vector<ExprNode*>         array_dimensions) : type_name       (type_name)
                                                                    , is_ref          (is_ref)
                                                                    , is_const        (is_const)
                                                                    , template_params (template_params)
                                                                    , pointer_depth   (pointer_depth)
                                                                    , array_dimensions(array_dimensions)
{

}

TypeInfo::TypeInfo(const TypeInfo& type_info) : type_name       (type_info.type_name)
                                              , is_ref          (type_info.is_ref)
                                              , is_const        (type_info.is_const)
                                              , template_params (type_info.template_params)
                                              , pointer_depth   (type_info.pointer_depth)
                                              , array_dimensions(type_info.array_dimensions)
{

}


TypeInfo::TypeInfo(TypeInfo&& type_info) : type_name       (std::move(type_info.type_name))
                                         , is_ref          (type_info.is_ref)
                                         , is_const        (type_info.is_const)
                                         , template_params (std::move(type_info.template_params))
                                         , pointer_depth   (type_info.pointer_depth)
                                         , array_dimensions(std::move(type_info.array_dimensions))
{

}

TypeInfo& TypeInfo::operator=(const TypeInfo& type_info)
{
    is_ref           = type_info.is_ref;
    is_const         = type_info.is_const;
    pointer_depth    = type_info.pointer_depth;
    type_name        = type_info.type_name;
    template_params  = type_info.template_params;
    array_dimensions = type_info.array_dimensions;
    return *this;
}

TypeInfo& TypeInfo::operator=(TypeInfo&& type_info)
{
    is_ref           = type_info.is_ref;
    is_const         = type_info.is_const;
    pointer_depth    = type_info.pointer_depth;
    type_name        = std::move(type_info.type_name);
    template_params  = std::move(type_info.template_params);
    array_dimensions = std::move(type_info.array_dimensions);
    return *this;
}

std::string TypeInfo::toString() const
{
    std::string res = type_name;

    if ( is_const )
        res = "const " + res;

    if ( !template_params.empty() )
    {
        res += "<";

        auto it = std::begin(template_params);

        if ( it -> which() == 0 )
            res += boost::get<ExprNode*>(*it) -> toString();
        else
            res += boost::get<TypeInfo>(*it).toString();

        for ( ++it; it != std::end(template_params); ++it )
        {
            const auto& tp = *it;

            res += ", ";
            if ( tp.which() == 0 )
                res += boost::get<ExprNode*>(tp) -> toString();
            else
                res += boost::get<TypeInfo>(tp).toString();
        }

        res += ">";
    }

    for ( int i = 0; i < pointer_depth; ++i )
        res += "*";

    for ( auto dim : array_dimensions )
        res += "[" + std::to_string(*dim -> getCompileTimeValue()) + "]";

    if ( is_ref )
        res += "&";

    return res;
}
