#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name) { }

void VariableNode::build_scope() { }

void VariableNode::check()
{
    Symbol *sym = this->getScope()->resolve(name);

    if ( sym == nullptr )
	throw SemanticError("No such symbol " + name);

    if ( sym->getSymbolType() != SymbolType::VARIABLE )
	throw SemanticError("'" + name + "' is not a variable.");

    variable = static_cast<VariableSymbol*>(sym);
}

void VariableNode::gen()
{
    Type *var_type = variable->getType();
    
    if ( var_type->isReference() )
    {
	if ( variable->isField() )
	{
	    VariableSymbol *sym = static_cast<VariableSymbol*>(getScope()->resolve("this"));

	    Scope *struc_scope = static_cast<StructSymbol*>(sym->getType());
	    
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(this->getScope()->getAddress(sym)) + "]");
	    CodeGen::emit("mov rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
	}
	else
	{
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->getAddress(variable)) + "]");
	}
    }    
    else if ( dynamic_cast<OverloadedFunctionSymbol*>(var_type) )
    {
	OverloadedFunctionSymbol *ov_func = dynamic_cast<OverloadedFunctionSymbol*>(var_type);
	
	OverloadedFunctionTypeInfo ov_func_type_info = ov_func->getTypeInfo();
	
	if ( ov_func_type_info.overloads.size() > 1 )
	{
	    FunctionSymbol *hint_type = static_cast<FunctionSymbol*>(GlobalHelper::getTypeHint(this));
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + name);
	   
	    variable = dynamic_cast<VariableSymbol*>(ov_func_type_info.symbols[hint_type->getTypeInfo()]);
	}
	else
	{
	    variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols.begin()->second);
	}

	CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(variable->getType())->getScopedTypedName() + "]");
    }
    else
    {
	if ( variable->isField() )
	{
	    Symbol *_this = getScope()->resolve("this");

	    if ( _this == nullptr || _this->getSymbolType() != SymbolType::VARIABLE )
		throw SemanticError("internal error");
	    
	    VariableSymbol *sym = static_cast<VariableSymbol*>(_this);

	    Scope *struc_scope = static_cast<StructSymbol*>(static_cast<ReferenceType*>(sym->getType())->getReferredType());
	    
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->getAddress(sym)) + "]");
	    CodeGen::emit("lea rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
	}
	else
	{
	    CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->getAddress(variable)) + "]");
	}
    }
}

Type* VariableNode::getType() const { return TypeHelper::getReferenceType(variable->getType()); }
bool VariableNode::isLeftValue() const { return true; }
