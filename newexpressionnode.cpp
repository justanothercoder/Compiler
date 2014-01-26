#include "newexpressionnode.hpp"

NewExpressionNode::NewExpressionNode(string name, vector<ExprNode*> params) : name(name), params(params) { }

void NewExpressionNode::build_scope()
{
    for ( auto i : params )
    {
	i->setScope(this->getScope());
	i->build_scope();
    }
}

void NewExpressionNode::check()
{
    Symbol *_type = this->getScope()->resolve(name);

    if ( _type->getSymbolType() != SymbolType::STRUCT )
	throw SemanticError("No such struct " + name);
	
    StructSymbol *type = static_cast<StructSymbol*>(_type);

    Symbol *_constr = type->resolve(name);
    if ( _constr->getSymbolType() != SymbolType::VARIABLE )	
	throw SemanticError("No constructor");

    VariableSymbol *_constructor = static_cast<VariableSymbol*>(_constr);
    if ( _constructor->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	throw SemanticError("No constructor");

    OverloadedFunctionSymbol *constructor = static_cast<OverloadedFunctionSymbol*>(_constructor->getType());

    vector<Type*> params_types;

    params_types.push_back(TypeHelper::getReferenceType(type));   

    std::for_each(std::begin(params), std::end(params), [](ExprNode *t) { t->check(); });
    std::transform(std::begin(params), std::end(params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });
    
    resolved_constructor = FunctionHelper::getViableOverload(constructor, params_types);

    if ( resolved_constructor == nullptr )
	throw SemanticError("No viable overload of '" + name + "'.");
	
    auto resolved_constructor_type_info = resolved_constructor->getTypeInfo();

    for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
    {
	if ( resolved_constructor_type_info.getParamType(i)->isReference() && !params[i - 1]->isLeftValue() )
	    throw SemanticError("parameter is not an lvalue.");
    }
}

void NewExpressionNode::gen()
{
    
}

Type* NewExpressionNode::getType() const
{
    return resolved_constructor->getTypeInfo().getReturnType();
}

bool NewExpressionNode::isLeftValue() const
{
    return false;
}
    
