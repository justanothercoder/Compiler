#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>
#include <vector>

class ExprNode;

class TypeInfo
{
public:   
    TypeInfo();
    TypeInfo(std::string type_name, bool is_ref, bool is_const, std::vector<ExprNode*> template_params = { }, int pointer_depth = 0);

	std::string type_name;
    bool is_ref, is_const;
	std::vector<ExprNode*> template_params;
	int pointer_depth;
};

#endif

