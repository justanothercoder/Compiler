#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>

#include "basescope.hpp"
#include "varallocator.hpp"
#include "tempallocator.hpp"
#include "templateinfo.hpp"
#include "globaltable.hpp"

class FunctionType;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    void accept(ScopeVisitor& visitor) override;

    Scope* getEnclosingScope() const override;
    std::string getScopeName() const override;

    VarAllocator& getVarAlloc() const override;
    TempAllocator& getTempAlloc() const override;

    const TemplateInfo& templateInfo() const override;
    bool isUnsafeBlock() const override;

    void defineBuiltInFunction(std::string name, FunctionType type);
    void defineBuiltInOperator(std::string name, FunctionType type);

private:

    mutable VarAllocator var_alloc;
    mutable TempAllocator temp_alloc;

    TemplateInfo template_info;
};

#endif
