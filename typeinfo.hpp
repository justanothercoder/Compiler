#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>
#include <vector>
#include <boost/variant.hpp>

class ExprNode;
class TypeInfo;

using TemplateParamInfo = boost::variant<ExprNode*, TypeInfo>;

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
           , int                            pointer_depth = 0
           , std::vector<ExprNode*>         array_dimensions = { }
           , std::string module_name = "");

    TypeInfo(const TypeInfo& type_info);
    TypeInfo& operator=(const TypeInfo& type_info);

    TypeInfo(TypeInfo&& type_info);
    TypeInfo& operator=(TypeInfo&& type_info);

    std::string toString() const;

    std::string type_name;
    bool is_ref, is_const;
    std::vector<TemplateParamInfo> template_params;
    int pointer_depth;
    std::vector<ExprNode*> array_dimensions;
    std::string module_name;
};
    
bool operator==(const TypeInfo& lhs, const TypeInfo& rhs);
bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs);

#endif
