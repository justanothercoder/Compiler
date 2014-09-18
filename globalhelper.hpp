#ifndef _GLOBALHELPER_HPP_
#define _GLOBALHELPER_HPP_

#include <string>
#include <map>

class AST;
class ExprNode;
class Type;
class Scope;
class Symbol;
class FunctionSymbol;
class VariableSymbol;

class GlobalHelper
{
public:
    
	static std::string getCodeOperatorName(std::string op);

	static int transformAddress(Scope *scope, int addr); 

	static std::map<FunctionSymbol*, bool> has_definition;

	static std::map<int, int> const_num_id;
	static std::map<VariableSymbol*, int> var_id;

	static void addConst(int value);
	static void addVar(VariableSymbol *var);
	
private:

    GlobalHelper();
};

#endif
