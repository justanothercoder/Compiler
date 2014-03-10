#include "typeinfo.hpp"

TypeInfo::TypeInfo() { }

TypeInfo::TypeInfo(string type_name, bool is_ref, const vector<ExprNode*>& template_params) : type_name(type_name), is_ref(is_ref), template_params(template_params) { }
