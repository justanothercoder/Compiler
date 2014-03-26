#include "globalhelper.hpp"

map<ExprNode*, Type*> GlobalHelper::type_hints;
map<Symbol*, bool> GlobalHelper::is_defined;
map<AST*, Scope*> GlobalHelper::ast_scopes;

Type* GlobalHelper::getTypeHint(ExprNode *expr) 
{ 
	auto it = type_hints.find(expr);
	return it == std::end(type_hints) ? nullptr : it->second;
}
void GlobalHelper::setTypeHint(ExprNode *expr, Type *type) { type_hints[expr] = type; }

Scope* GlobalHelper::getASTScope(AST *t) { return ast_scopes.at(t); }
void GlobalHelper::setASTScope(AST *t, Scope *sc) { ast_scopes[t] = sc; }
	
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
    
int GlobalHelper::isAlreadyDefined(Symbol *sym) { return is_defined.find(sym) != std::end(is_defined) && is_defined[sym] == true; }
void GlobalHelper::setDefined(Symbol *sym) { is_defined[sym] = true; }
