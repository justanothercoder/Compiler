#include "typeinfo.hpp"

TypeInfo::TypeInfo() { }

TypeInfo::TypeInfo(string type_name
		         , bool is_ref
				 , bool is_const
				 , vector<ExprNode*> template_params) : type_name(type_name)
												      , is_ref(is_ref)
													  , is_const(is_const)
													  , template_params(template_params) 
{

}
