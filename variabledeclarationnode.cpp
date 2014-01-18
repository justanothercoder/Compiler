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
    if ( !is_field )
    {
	Symbol *_type = this->getScope()->resolve(type_info.getTypeName());
        
	StructSymbol *type = dynamic_cast<StructSymbol*>(_type);

	if ( type == nullptr )
	    throw SemanticError("No such struct " + type_info.getTypeName());

	VariableSymbol *_constructor = dynamic_cast<VariableSymbol*>(type->resolve(type_info.getTypeName()));

	if ( _constructor == nullptr )
	    throw SemanticError("No constructor");
    
	OverloadedFunctionSymbol *constructor = dynamic_cast<OverloadedFunctionSymbol*>(_constructor->getType());

	if ( constructor == nullptr )
	    throw SemanticError("No constructor");

	vector<Type*> params_types;
	params_types.push_back(TypeHelper::getReferenceType(definedSymbol->getType()));   
	for ( auto it : constructor_call_params )
	{
	    it->check();
	    params_types.push_back(it->getType());
	}

	resolved_constructor = FunctionHelper::getViableOverload(constructor, params_types);

	if ( resolved_constructor == nullptr )
	    throw SemanticError("No viable overload of '" + type_info.getTypeName() + "'.");
	
	auto resolved_constructor_type_info = resolved_constructor->getTypeInfo();

	for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
	{
	    if ( dynamic_cast<ReferenceType*>(resolved_constructor_type_info.getParamType(i)) && !constructor_call_params[i - 1]->isLeftValue() )
		throw SemanticError("parameter is not an lvalue.");
	}
    }
}

void VariableDeclarationNode::gen()
{
    if ( !is_field )
    {
      
	CodeGen::emit("lea rdi, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
	CodeGen::emit("sub rsp, " + std::to_string(definedSymbol->getType()->getSize()));

	CodeGen::emit("lea rsi, [_~" + resolved_constructor->getScopedTypedName() + "]");

	FunctionHelper::genCallCode(resolved_constructor, constructor_call_params);
/*	
	int paramsSize = 0;

	auto resolved_constructor_type_info = resolved_constructor->getTypeInfo();
    
	for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
	{
	    constructor_call_params[i - 1]->gen();
	    if ( dynamic_cast<ReferenceType*>(resolved_constructor_type_info.getParamType(i)) )
	    {	    
		CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rax");
		paramsSize += GlobalConfig::int_size;
	    }
	    else
	    {
		for ( int j = 0; j < resolved_constructor_type_info.getParamType(i)->getSize(); j += GlobalConfig::int_size, paramsSize += GlobalConfig::int_size )
		{	    
		    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rbx");
		}
	    }
	}

	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rdi");
	paramsSize += GlobalConfig::int_size;
	
	CodeGen::emit("sub rsp, " + std::to_string(paramsSize));
	CodeGen::emit("call _~" + resolved_constructor->getScopedTypedName());
	CodeGen::emit("add rsp, " + std::to_string(paramsSize));
*/	
    }
}
