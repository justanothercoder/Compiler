#ifndef _NOVIABLEOVERLOADERROR_HPP_
#define _NOVIABLEOVERLOADERROR_HPP_

#include <string>
#include <vector>
#include "semanticerror.hpp"
#include "variabletype.hpp"
#include "valueinfo.hpp"

class NoViableOverloadError : public SemanticError
{
public:

    NoViableOverloadError(std::string name, std::vector<VariableType> params_types);
    NoViableOverloadError(std::string name, std::vector<ValueInfo> arguments);

private:
    static std::string paramsToString(std::vector<VariableType> params_types);
    static std::string paramsToString(std::vector<ValueInfo> params_types);

};

#endif
