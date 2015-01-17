#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <memory>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "exprnode.hpp"
#include "typemodifier.hpp"

class TypeInfo;

using TemplateParamInfo = boost::variant< std::shared_ptr<ExprNode>, TypeInfo >;
using ParamInfo = std::pair<std::string, TypeInfo>;

class TypeInfo
{
    friend bool operator==(const TypeInfo& lhs, const TypeInfo& rhs);
    friend bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs);

public:
    TypeInfo();
    TypeInfo(std::string                    type_name
           , bool                           is_ref
           , bool                           is_const
           , std::vector<TemplateParamInfo> template_params = { }
           , std::vector<TypeModifier>      modifiers = { }
           , std::string module_name = "");

    TypeInfo(const TypeInfo& type_info);
    TypeInfo& operator=(const TypeInfo& type_info);

    TypeInfo(TypeInfo&& type_info);
    TypeInfo& operator=(TypeInfo&& type_info);

    std::string toString() const;

    const std::string& name() const;
    void name(const std::string& name);

    bool isRef() const;
    bool isConst() const;
    const std::vector<TypeModifier>& modifiers() const;
    const std::vector<TemplateParamInfo>& templateParams() const;
    const std::string& moduleName() const;

private:

    std::string type_name;
    bool is_ref, is_const;
    std::vector<TemplateParamInfo> template_params;
    std::vector<TypeModifier> modifiers_;
    std::string module_name;
};
    
bool operator==(const TypeInfo& lhs, const TypeInfo& rhs);
bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs);

TypeInfo makeTypeInfo(VariableType type);

#endif
