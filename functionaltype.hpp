#ifndef _FUNCTIONALTYPE_HPP_
#define _FUNCTIONALTYPE_HPP_

#include <map>
#include "valueinfo.hpp"
#include "type.hpp"
#include "callinfo.hpp"
#include "functiontypeinfo.hpp"
#include "templateparam.hpp"

class TemplateSymbol;

class FunctionalType : public Type
{
public:
    
    size_t sizeOf() const override;
    
    bool isConvertableTo(const Type *type) const override;
    boost::optional<int> rankOfConversion(const Type *type) const override;
    const FunctionSymbol* getConversionTo(const Type *type) const override;

    virtual CallInfo resolveCall(std::vector<ValueInfo> arguments) const = 0;
};

bool checkValues(std::vector<ValueInfo> arguments, std::vector<VariableType> params);
std::vector<ConversionInfo> getConversions(std::vector<ValueInfo> arguments, std::vector<VariableType> params);
ConversionInfo getConversionInfo(const Type *lhs, const Type *rhs);

boost::optional< std::map<std::string, TemplateParam> > makeMappingOfParams(const TemplateSymbol* tmpl
                                                                          , const std::vector<ParamInfo>& formal_params
                                                                          , FunctionTypeInfo arguments);

#endif
