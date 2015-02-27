#include "functionalsymbol.hpp"
#include "comp.hpp"
#include "globalconfig.hpp"
#include "scope.hpp"

bool FunctionalSymbol::isMethod()      const { return traits().is_method; }
bool FunctionalSymbol::isOperator()    const { return traits().is_operator; }
bool FunctionalSymbol::isConstructor() const { return traits().is_constructor; }

std::string FunctionalSymbol::getTypedName() const
{
    auto name = getName();
    auto res = (isOperator() ? Comp::config().getCodeOperatorName(name) : name);

    for ( auto param_type : type().typeInfo().params() )
        res += "_" + param_type.getName();

    return res;
}

std::string FunctionalSymbol::getScopedTypedName() const
{
    return scope -> getScopeName() + "_" + getTypedName();
}
