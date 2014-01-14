#include "globalhelper.hpp"

map<ExprNode*, Type*> GlobalHelper::type_hints = map<ExprNode*, Type*>();

Type* GlobalHelper::getTypeHint(ExprNode *expr)
{
    auto it = type_hints.find(expr);

    if ( it == std::end(type_hints) )	
	return nullptr;

    return it->second;
}

void GlobalHelper::setTypeHint(ExprNode *expr, Type *type)
{
    type_hints[expr] = type;
}
