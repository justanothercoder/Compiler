#ifndef _CODEOBJECT_HPP_
#define _CODEOBJECT_HPP_

#include <iostream>
#include <string>
#include <algorithm>

using std::string;

#include "type.hpp"
#include "builtins.hpp"
#include "callinfo.hpp"

class FunctionSymbol;

class CodeObject
{
public:

	void emit(string text);

	void genCallCode(CallInfo call_info, std::vector<ExprNode*> params, CodeObject& genThis, bool thisIsRef);
	void genParam(ExprNode *param, ConversionInfo conv_info, const FunctionSymbol *copy_constr);

	void pushOnStack(size_t size, int offset);

	void genCopy(FunctionSymbol *copy_constr, CodeObject& genThis, CodeObject& copied);
	void genDefaultConstructorCall(FunctionSymbol *default_constr, CodeObject& genThis);

	void gen() const;	
	std::string getCode() const;

private:

	std::string code;
};

#endif
