#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <iostream>
#include <string>

using std::cout;
using std::string;

#include "type.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"

class CodeGen
{
public:

    static void emit(string text);

    static void construct_object(Type *type, FunctionSymbol *constructor, const vector<ExprNode*>& params);

    static void genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params);
};

#endif
