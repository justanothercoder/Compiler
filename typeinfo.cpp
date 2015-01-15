#include "typeinfo.hpp"
#include "exprnode.hpp"

TypeInfo::TypeInfo() { }

TypeInfo::TypeInfo(std::string                    type_name
                 , bool                           is_ref
                 , bool                           is_const
                 , std::vector<TemplateParamInfo> template_params
                 , std::vector<TypeModifier>      modifiers
                 , std::string module_name                        ) : type_name      (type_name)
                                                                    , is_ref         (is_ref)
                                                                    , is_const       (is_const)
                                                                    , template_params(template_params)
                                                                    , modifiers_     (modifiers)
                                                                    , module_name    (module_name)
{

}

TypeInfo::TypeInfo(const TypeInfo& type_info) : type_name      (type_info.type_name)
                                              , is_ref         (type_info.is_ref)
                                              , is_const       (type_info.is_const)
                                              , template_params(type_info.template_params)
                                              , modifiers_     (type_info.modifiers_)
                                              , module_name    (type_info.module_name)
{

}


TypeInfo::TypeInfo(TypeInfo&& type_info) : type_name      (std::move(type_info.type_name))
                                         , is_ref         (type_info.is_ref)
                                         , is_const       (type_info.is_const)
                                         , template_params(std::move(type_info.template_params))
                                         , modifiers_     (std::move(type_info.modifiers_))
                                         , module_name    (std::move(type_info.module_name))
{

}

TypeInfo& TypeInfo::operator=(const TypeInfo& type_info)
{
    type_name       = type_info.type_name;
    is_ref          = type_info.is_ref;
    is_const        = type_info.is_const;
    template_params = type_info.template_params;
    modifiers_      = type_info.modifiers_;
    module_name     = type_info.module_name;
    return *this;
}

TypeInfo& TypeInfo::operator=(TypeInfo&& type_info)
{
    type_name       = std::move(type_info.type_name);
    is_ref          = type_info.is_ref;
    is_const        = type_info.is_const;
    template_params = std::move(type_info.template_params);
    modifiers_      = std::move(type_info.modifiers_);
    module_name     = std::move(type_info.module_name);
    return *this;
}

std::string TypeInfo::toString() const
{
    auto res = type_name;

    if ( !module_name.empty() )
        res = module_name + "." + res;

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
const std::vector<TemplateParamInfo>& TypeInfo::templateParams() const { return template_params; }
const std::string& TypeInfo::moduleName() const { return module_name; }

bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{
    return lhs.type_name       == rhs.type_name
        && lhs.is_ref          == rhs.is_ref
        && lhs.is_const        == rhs.is_const
        && lhs.template_params == rhs.template_params
        && lhs.modifiers_      == rhs.modifiers_
        && lhs.module_name     == rhs.module_name;
}

bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) { return !(lhs == rhs); }    
