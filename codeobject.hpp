#ifndef _CODEOBJECT_HPP_
#define _CODEOBJECT_HPP_

#include <iostream>
#include <string>
#include <algorithm>

using std::string;

#include "type.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "templateinfo.hpp"
#include "builtins.hpp"
#include "callinfo.hpp"

#include "variablenode.hpp"
#include "dotnode.hpp"

class CodeObject
{
public:

	void emit(string text);

	void genCallCode(CallInfo call_info, vector<ExprNode*> params, const TemplateInfo& template_info, CodeObject& genThis);

	void genParam(ExprNode *param, ConversionInfo conv_info, FunctionSymbol *copy_constr, const TemplateInfo& template_info);

	void pushOnStack(size_t size, int offset);

	void gen() const;	
	std::string getCode() const;

private:

	std::string code;
};

#endif
