#ifndef _STRUCTSCOPE_HPP_
#define _STRUCTSCOPE_HPP_

#include "basescope.hpp"
#include "templateinfo.hpp"

class StructScope : public BaseScope
{
    friend class VariableSymbolDefine;

public:

    StructScope(std::string scope_name, Scope* enclosing_scope);

    Scope* enclosingScope() const override;    
    std::string getScopeName() const override;

    void accept(ScopeVisitor& visitor) override;

private:
    
    std::string scope_name;
    Scope* enclosing_scope;

protected:

    int type_size;
};

#endif
