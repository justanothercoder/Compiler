#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>

using std::string;

struct TypeInfo
{
    TypeInfo(string type_name, bool isRef);
    
    const string type_name;
    const bool isRef;
};

#endif
