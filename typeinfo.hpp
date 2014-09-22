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
public:   
    TypeInfo();
    TypeInfo(std::string type_name, bool is_ref, bool is_const, std::vector<TemplateParamInfo> template_params = { }, int pointer_depth = 0);

	std::string toString() const;

	std::string type_name;
    bool is_ref, is_const;
	std::vector<TemplateParamInfo> template_params;
	int pointer_depth;
};

#endif

