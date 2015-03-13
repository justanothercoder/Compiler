#ifndef _TEMPLATESPECIALIZATIONERROR_HPP_
#define _TEMPLATESPECIALIZATIONERROR_HPP_

#include "semanticerror.hpp"
#include "templatesymbol.hpp"
#include "templateargument.hpp"

class TemplateSpecializationError : public SemanticError
{
public:

    TemplateSpecializationError(const TemplateSymbol* sym, const TemplateArguments& args);

private:

    std::string makeErrorMessage(const TemplateSymbol* sym, const TemplateArguments& args);
};

#endif
