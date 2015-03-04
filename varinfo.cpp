#include "varinfo.hpp"
#include "type.hpp"

VarInfo::VarInfo(std::string name, VariableType type) : name_(name), type_(type) { }

const std::string& VarInfo::name() const { return name_; }
VariableType       VarInfo::type() const { return type_; }

std::string VarInfo::toString() const { return type_.getName() + name_; }
