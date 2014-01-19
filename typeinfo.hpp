#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <string>

using std::string;

class TypeInfo
{
public:   
    TypeInfo();
    TypeInfo(string type_name, bool is_ref);

    string getTypeName() const;
    bool getIsRef() const;

private:
    string type_name;
    bool is_ref;
};

#endif
