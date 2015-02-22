#ifndef _GLOBALSCOPE_HPP_
#define _GLOBALSCOPE_HPP_

#include <map>
#include "basescope.hpp"
#include "templateinfo.hpp"

class FunctionType;

class GlobalScope : public BaseScope
{
public:

    GlobalScope();

    Scope* enclosingScope() const override;
    std::string getScopeName() const override;

    bool isUnsafeBlock() const override;

    void defineBuiltInFunction(std::string name, FunctionType type);
    void defineBuiltInOperator(std::string name, FunctionType type);
};

#endif
