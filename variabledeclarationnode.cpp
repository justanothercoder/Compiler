#include "variabledeclarationnode.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, TypeInfo type_info, bool is_field, const vector<ExprNode*>& constructor_call_params) : name(name), type_info(type_info), is_field(is_field), constructor_call_params(constructor_call_params) { definedSymbol = new VariableSymbol(name, nullptr, (is_field ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE)); }

VariableDeclarationNode::~VariableDeclarationNode() { delete definedSymbol; }

Symbol* VariableDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void VariableDeclarationNode::build_scope()
{
    for ( auto i : constructor_call_params )
    {
	i->setScope(this->getScope());
	i->build_scope();
    }
}

void VariableDeclarationNode::define()
{
    Type *var_type = TypeHelper::fromTypeInfo(type_info, this->getScope());
    definedSymbol->setType(var_type);
    this->getScope()->define(definedSymbol);
}

void VariableDeclarationNode::check()
{
    OverloadedFunctionSymbol *constructor = dynamic_cast<OverloadedFunctionSymbol*>(this->getScope()->resolve(type_info.getTypeName()));

    vector<Type*> params_types;
    for ( auto it : constructor_call_params )
    {
	it->check();
	params_types.push_back(it->getType());
    }

    auto overloads = FunctionHelper::getBestOverload(constructor->getTypeInfo().overloads, params_types);

    if ( overloads.empty() )
	throw SemanticError("No viable overload of '" + constructor->getTypeInfo().getName() + "'.");

    auto resolved_constructor_type_info = *std::begin(overloads);

    resolved_constructor = constructor->getTypeInfo().symbols[resolved_constructor_type_info];

    for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
    {
	if ( dynamic_cast<ReferenceType*>(resolved_constructor_type_info.getParamType(i)) && !constructor_call_params[i - 1]->isLeftValue() )
	    throw SemanticError("parameter is not an lvalue.");
    }
}

void VariableDeclarationNode::gen()
{
    if ( !is_field )
	CodeGen::emit("sub rsp, " + std::to_string(static_cast<VariableSymbol*>(definedSymbol)->getType()->getSize()));
}
