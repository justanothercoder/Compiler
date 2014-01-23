#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name) { }

void VariableNode::build_scope() { }

void VariableNode::check()
{
    Symbol *sym = this->getScope()->resolve(name);

    if ( sym == nullptr )
	throw SemanticError("No such symbol " + name);

    if ( sym->getSymbolType() == SymbolType::STRUCT )
    {
	Symbol *constr = static_cast<StructSymbol*>(sym)->resolve(name);
	if ( constr->getSymbolType() != SymbolType::VARIABLE )
	    throw SemanticError("'" + name + "' is not a variable.");

	if ( static_cast<VariableSymbol*>(constr)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	    throw SemanticError("'" + name + "' is not a variable.");

	if ( !static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(constr)->getType())->isConstructor() )
	    throw SemanticError("'" + name + "' is not a variable.");

	sym = constr;
    }
    else
    {
	if ( sym->getSymbolType() != SymbolType::VARIABLE )
	    throw SemanticError("'" + name + "' is not a variable.");
    }

    variable = static_cast<VariableSymbol*>(sym);
}

void VariableNode::gen()
{
    Type *var_type = variable->getType();
    
    if ( var_type->isReference() )
    {
	if ( variable->isField() )
	{
	    VariableSymbol *sym = static_cast<VariableSymbol*>(this->getScope()->resolve("this"));

	    Scope *struc_scope = static_cast<StructSymbol*>(sym->getType());
	    
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(this->getScope()->getAddress(sym)) + "]");
	    CodeGen::emit("mov rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
	}
	else
	{
	    CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->getAddress(variable)) + "]");
	}
    }    
    else if ( var_type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
    {
	OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(var_type);
	
	auto ov_func_type_info = ov_func->getTypeInfo();
	
	if ( ov_func_type_info.overloads.size() > 1 )
	{
	    FunctionSymbol *hint_type = static_cast<FunctionSymbol*>(GlobalHelper::getTypeHint(this));
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + name);

	    auto type_info = hint_type->getTypeInfo();

	    if ( ov_func_type_info.symbols.find(type_info) == std::end(ov_func_type_info.symbols) )
	    {
		auto sym = FunctionHelper::getViableOverload(ov_func, type_info.getParamsTypes());
		variable = new VariableSymbol(ov_func->getName(), sym);
	    }
	    else
		variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols[type_info]);
	}
	else
	{
	    variable = new VariableSymbol(ov_func->getName(), std::begin(ov_func_type_info.symbols)->second);
	}

	CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(variable->getType())->getScopedTypedName() + "]");
    }
    else
    {
	if ( variable->isField() )
	{
	    Symbol *_this = this->getScope()->resolve("this");

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
