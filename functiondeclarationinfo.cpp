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

std::string FunctionDeclarationInfo::toString() const
{
    std::string result = "";

    result += "(";
    if ( !formal_parameters.empty() )
    {
        auto it = std::begin(formal_parameters);
        result += it -> second.toString() + " " + it -> first;
        for ( ++it; it != std::end(formal_parameters); ++it )
            result += ", " + it -> second.toString() + " " + it -> first;
    }
    result += ")";
    result += " " + return_type_info.toString();

    return result;
}
