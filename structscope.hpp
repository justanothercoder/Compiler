#ifndef _STRUCTSCOPE_HPP_
#define _STRUCTSCOPE_HPP_

#include "basescope.hpp"
#include "templateinfo.hpp"

class StructScope : public BaseScope
{
public:

    StructScope(std::string scope_name, Scope* enclosing_scope);

    Scope* enclosingScope() const override;    
    std::string getScopeName() const override;

    bool isUnsafeBlock() const override;

    int type_size;

    const std::string& structName() const;

private:
    
    std::string scope_name;
    Scope* enclosing_scope;

    std::string name;
};

#endif
