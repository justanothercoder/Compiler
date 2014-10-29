#include "globaltable.hpp"
#include "scope.hpp"
#include "varallocator.hpp"
#include "globalconfig.hpp"

int GlobalTable::transformAddress(Scope *scope, int addr)
{
	return scope -> getVarAlloc().getSpace() + GlobalConfig::int_size + addr; 
}
	
void GlobalTable::addConst(int value)
{
	static int num_value_id = 0;

	if ( const_num_id.find(value) != std::end(const_num_id) )
		return;

	++num_value_id;	

	const_num_id[value] = num_value_id;
	id_to_num[num_value_id] = value;
}
	
void GlobalTable::addVar(VariableSymbol *var)
{
	static int variable_id = 0;

	if ( id_by_var.find(var) != std::end(id_by_var) )
		return;

	++variable_id;

	id_by_var[var] = variable_id;
	var_by_id[variable_id] = var;
}

void GlobalTable::addFunc(const FunctionSymbol *func)
{
	static int func_id = 0;

	if ( id_by_func.find(func) != std::end(id_by_func) )
		return;

	++func_id;

	func_by_id[func_id] = func;
	id_by_func[func] = func_id;
}
	
void GlobalTable::addStr(std::string s)
{
	static int str_id = 0;

	if ( id_by_string.find(s) != std::end(id_by_string) )
		return;

	++str_id;

	string_by_id[str_id] = s;
	id_by_string[s] = str_id;
}
