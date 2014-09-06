#include "typeinfo.hpp"

TypeInfo::TypeInfo() 
{

}

TypeInfo::TypeInfo(std::string type_name
		         , bool is_ref
				 , bool is_const
				 , std::vector<ExprNode*> template_params
				 , int pointer_depth) : type_name(type_name)
									  , is_ref(is_ref)
									  , is_const(is_const)
									  , template_params(template_params)
									  , pointer_depth(pointer_depth)
{

}
