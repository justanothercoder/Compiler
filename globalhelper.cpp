#include "globalhelper.hpp"

map<ExprNode*, Type*> GlobalHelper::type_hints = map<ExprNode*, Type*>();
map<Symbol*, int> GlobalHelper::definition_time = map<Symbol*, int>();
map<AST*, Scope*> GlobalHelper::ast_scopes = map<AST*, Scope*>();

Type* GlobalHelper::getTypeHint(ExprNode *expr)
{
	auto it = type_hints.find(expr);

	if ( it == std::end(type_hints) )	
		return nullptr;

	return it->second;
}

void GlobalHelper::setTypeHint(ExprNode *expr, Type *type) { type_hints[expr] = type; }

Scope* GlobalHelper::getASTScope(const AST *t)
{
	auto it = ast_scopes.find(const_cast<AST*>(t));

	if ( it == std::end(ast_scopes) )
		return nullptr;

	return it->second;
}

void GlobalHelper::setASTScope(AST *t, Scope *sc) { ast_scopes[t] = sc; }
	
std::string GlobalHelper::getCodeOperatorName(std::string op)
{
    if      ( op == "operator()" ) return "operatorcall";
	else if ( op == "operator[]" ) return "operatorelem";
	else if ( op == "operator+"  ) return "operatorplus";
	else if ( op == "operator-"  ) return "operatorminus";
	else if ( op == "operator="  ) return "operatorassign";
	else if ( op == "operator*"  ) return "operatormul";
}
