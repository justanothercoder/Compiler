#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "basescope.hpp"
#include "templateinfo.hpp"
#include "globaltable.hpp"

class FunctionType;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    void accept(ScopeVisitor& visitor) override;

    Scope* enclosingScope() const override;
    std::string getScopeName() const override;

    const TemplateInfo& templateInfo() const override;
    bool isUnsafeBlock() const override;

    void defineBuiltInFunction(std::string name, FunctionType type);
    void defineBuiltInOperator(std::string name, FunctionType type);

private:

    TemplateInfo template_info;
};

#endif
