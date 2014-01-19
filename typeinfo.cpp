#include "typeinfo.hpp"

TypeInfo::TypeInfo() { }

TypeInfo::TypeInfo(string type_name, bool is_ref) : type_name(type_name), is_ref(is_ref) { }

string TypeInfo::getTypeName() const { return type_name; }

bool TypeInfo::getIsRef() const { return is_ref; }
