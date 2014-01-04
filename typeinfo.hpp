#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>

using std::string;

struct TypeInfo
{
    TypeInfo(string type_name, bool is_ref);
    
    const string type_name;
    const bool is_ref;
};

#endif
