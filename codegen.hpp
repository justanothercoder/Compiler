#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <iostream>
#include <string>
#include <algorithm>

using std::string;

#include "type.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"
#include "templatestructsymbol.hpp"
#include "builtins.hpp"

class CodeGen
{
public:

    static void emit(string text);

    static void construct_object(FunctionSymbol *constructor, const vector<ExprNode*>& params, const TemplateStructSymbol *template_sym = nullptr, std::vector<ExprNode*> template_expr = { });

    static void genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params, const TemplateStructSymbol *template_sym = nullptr, std::vector<ExprNode*> template_expr = { });

    static void pushOnStack(unsigned int size, int offset);
    static void genConversion(FunctionSymbol *conv);
	static void genCopy(FunctionSymbol *copy_constructor, int stack_offset, Type *type);
};

#endif
