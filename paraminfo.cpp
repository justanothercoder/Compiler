#include "paraminfo.hpp"

ParamInfo::ParamInfo(std::string name, TypeInfo type_info) : name_(name), type_info(type_info) { }

const std::string& ParamInfo::name() const { return name_; }
const TypeInfo& ParamInfo::typeInfo() const { return type_info; }

TypeInfo& ParamInfo::typeInfo() { return type_info; }

std::string ParamInfo::toString() const { return type_info.toString() + " " + name_; }
