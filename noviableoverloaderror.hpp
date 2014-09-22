#ifndef _NOVIABLEOVERLOADERROR_HPP_
#define _NOVIABLEOVERLOADERROR_HPP_

#include <string>
#include <vector>
#include "type.hpp"
#include "semanticerror.hpp"

class NoViableOverloadError : public SemanticError
{
public:
	
	NoViableOverloadError(std::string name, std::vector<const Type*> params_types);
	
private:
	static std::string paramsToString(std::vector<const Type*> params_types);

};

#endif
