#include "globalhelper.hpp"
#include "scope.hpp"
#include "varallocator.hpp"
#include "globalconfig.hpp"

std::map<FunctionSymbol*, bool> GlobalHelper::has_definition;

std::map<int, int> GlobalHelper::const_num_id;
std::map<int, int> GlobalHelper::id_to_num;

std::map<VariableSymbol*, int> GlobalHelper::id_by_var;
std::map<int, VariableSymbol*> GlobalHelper::var_by_id;
	
std::map<int, std::string> GlobalHelper::label_name;
	
std::map<const FunctionSymbol*, int> GlobalHelper::id_by_func;
std::map<int, const FunctionSymbol*> GlobalHelper::func_by_id;
	
std::map<std::string, int> GlobalHelper::id_by_string;
std::map<int, std::string> GlobalHelper::string_by_id;

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

	if ( id_by_var.find(var) != std::end(id_by_var) )
		return;

	++variable_id;

	id_by_var[var] = variable_id;
	var_by_id[variable_id] = var;
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

	if ( id_by_string.find(s) != std::end(id_by_string) )
		return;

	++str_id;

	string_by_id[str_id] = s;
	id_by_string[s] = str_id;
}
