#ifndef _FUNCTIONDECLARATIONINFO_HPP_
#define _FUNCTIONDECLARATIONINFO_HPP_

#include <vector>
#include "typeinfo.hpp"

class FunctionDeclarationInfo
{
public:    

    FunctionDeclarationInfo(TypeInfo return_type_info, std::vector<ParamInfo> formal_parameters);

    FunctionDeclarationInfo(const FunctionDeclarationInfo& info) = default;
    FunctionDeclarationInfo(FunctionDeclarationInfo&& info)      = default;

    FunctionDeclarationInfo& operator=(const FunctionDeclarationInfo& info) = default;
    FunctionDeclarationInfo& operator=(FunctionDeclarationInfo&& info)      = default;

    TypeInfo& returnTypeInfo();
    const TypeInfo& returnTypeInfo() const;
    
    std::vector<ParamInfo>& formalParams();
    const std::vector<ParamInfo>& formalParams() const;

    std::string toString() const;

private:

    TypeInfo return_type_info;
    std::vector<ParamInfo> formal_parameters;
};

#endif
