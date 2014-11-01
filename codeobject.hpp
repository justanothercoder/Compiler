#ifndef _CODEOBJECT_HPP_
#define _CODEOBJECT_HPP_

#include <iostream>
#include <string>
#include <algorithm>

#include "callinfo.hpp"

class ExprNode;
class FunctionSymbol;

class CodeObject
{
public:

    void emit(std::string text);

    void gen() const;
    std::string getCode() const;

private:

    std::string code;
};

#endif
