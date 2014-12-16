#include "functiondeclarationinfo.hpp"

FunctionDeclarationInfo::FunctionDeclarationInfo(TypeInfo return_type_info, std::vector<ParamInfo> formal_parameters) 
                                                                                                    : return_type_info (return_type_info)
                                                                                                    , formal_parameters(formal_parameters)
{

}

FunctionDeclarationInfo::FunctionDeclarationInfo(FunctionDeclarationInfo&& info) : return_type_info(std::move(info.return_type_info))
                                                                                 , formal_parameters(std::move(info.formal_parameters))
{

}

FunctionDeclarationInfo& FunctionDeclarationInfo::operator=(FunctionDeclarationInfo&& info)
{
    return_type_info  = std::move(info.return_type_info);
    formal_parameters = std::move(info.formal_parameters);
    return *this;
}

TypeInfo& FunctionDeclarationInfo::returnTypeInfo()             { return return_type_info; } 
const TypeInfo& FunctionDeclarationInfo::returnTypeInfo() const { return return_type_info; }

std::vector<ParamInfo>& FunctionDeclarationInfo::formalParams()             { return formal_parameters; }
const std::vector<ParamInfo>& FunctionDeclarationInfo::formalParams() const { return formal_parameters; }

