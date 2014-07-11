#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>
#include <vector>

class ExprNode;

using std::string;
using std::vector;

class TypeInfo
{
public:   
    TypeInfo();
    TypeInfo(string type_name, bool is_ref, bool is_const, vector<ExprNode*> template_params = { });

    string type_name;
    bool is_ref, is_const;
    vector<ExprNode*> template_params;
};

#endif

