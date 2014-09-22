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
	static std::map<int, int> id_to_num;

	static std::map<VariableSymbol*, int> var_id;
	static std::map<int, VariableSymbol*> id_to_var;

	static std::map<int, std::string> label_name;

	static std::map<const FunctionSymbol*, int> id_by_func;
	static std::map<int, const FunctionSymbol*> func_by_id;

	static std::map<std::string, int> string_to_id;
	static std::map<int, std::string> id_to_string;

	static void addConst(int value);
	static void addVar(VariableSymbol *var);
	static void addFunc(const FunctionSymbol *func);
	static void addStr(std::string s);
	
private:

    GlobalHelper();
};

#endif
