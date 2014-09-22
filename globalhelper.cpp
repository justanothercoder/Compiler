#include "globalhelper.hpp"
#include "scope.hpp"
#include "varallocator.hpp"
#include "globalconfig.hpp"
	
std::map<FunctionSymbol*, bool> GlobalHelper::has_definition;

std::map<int, int> GlobalHelper::const_num_id;
std::map<int, int> GlobalHelper::id_to_num;

std::map<VariableSymbol*, int> GlobalHelper::var_id;
std::map<int, VariableSymbol*> GlobalHelper::id_to_var;
	
std::map<int, std::string> GlobalHelper::label_name;
	
std::map<const FunctionSymbol*, int> GlobalHelper::id_by_func;
std::map<int, const FunctionSymbol*> GlobalHelper::func_by_id;
	
std::map<std::string, int> GlobalHelper::string_to_id;
std::map<int, std::string> GlobalHelper::id_to_string;

std::string GlobalHelper::getCodeOperatorName(std::string op)
{
    if      ( op == "operator()" ) return "operatorcall";
	else if ( op == "operator[]" ) return "operatorelem";
	else if ( op == "operator+"  ) return "operatorplus";
	else if ( op == "operator-"  ) return "operatorminus";
	else if ( op == "operator="  ) return "operatorassign";
	else if ( op == "operator*"  ) return "operatormul";
	else if ( op == "operator/"  ) return "operatordiv";
	else if ( op == "operator%"  ) return "operatormod";
	else if ( op == "operator!"  ) return "operatornot";
	else if ( op == "operator==" ) return "operatoreq";
	else if ( op == "operator!=" ) return "operatorneq";
	else if ( op == "operator&&" ) return "operatorand";
	else if ( op == "operator||" ) return "operatoror";
	else if ( op.substr(0, 8) == "operator" ) return "operator~" + op.substr(9);
	else throw;
}

int GlobalHelper::transformAddress(Scope *scope, int addr)
{
	return scope -> getVarAlloc().getSpace() + GlobalConfig::int_size + addr; 
}
	
void GlobalHelper::addConst(int value)
{
	static int num_value_id = 0;

	if ( const_num_id.find(value) != std::end(const_num_id) )
		return;

	++num_value_id;	

	const_num_id[value] = num_value_id;
	id_to_num[num_value_id] = value;
}
	
void GlobalHelper::addVar(VariableSymbol *var)
{
	static int variable_id = 0;

	if ( var_id.find(var) != std::end(var_id) )
		return;

	++variable_id;

	var_id[var] = variable_id;
	id_to_var[variable_id] = var;
}

void GlobalHelper::addFunc(const FunctionSymbol *func)
{
	static int func_id = 0;

	if ( id_by_func.find(func) != std::end(id_by_func) )
		return;

	++func_id;

	func_by_id[func_id] = func;
	id_by_func[func] = func_id;
}
	
void GlobalHelper::addStr(std::string s)
{
	static int str_id = 0;

	if ( string_to_id.find(s) != std::end(string_to_id) )
		return;

	++str_id;

	id_to_string[str_id] = s;
	string_to_id[s] = str_id;
}
