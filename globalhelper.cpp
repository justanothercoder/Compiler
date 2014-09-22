#include "globalhelper.hpp"
#include "scope.hpp"
#include "varallocator.hpp"
#include "globalconfig.hpp"
	
std::map<FunctionSymbol*, bool> GlobalHelper::has_definition;

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
