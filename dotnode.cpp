#include "dotnode.hpp"

DotNode::DotNode(ExprNode *base, string member_name) : base(base), member_name(member_name) { }

void DotNode::build_scope()
{
    base->setScope(getScope());
    base->build_scope();    
}

void DotNode::check()
{
    base->check();

    Type *_base_type = base->getType();

    if ( _base_type->isReference() )
	_base_type = static_cast<ReferenceType*>(_base_type)->getReferredType();
    
    base_type = dynamic_cast<StructSymbol*>(_base_type);

    if ( base_type == nullptr )
	throw SemanticError("left side of '.' is not a struct instance.");

    member = base_type->resolveMember(member_name);

    if ( member == nullptr )
	throw SemanticError(member_name + " is not member of " + base_type->getName());
}

void DotNode::gen()
{
    base->gen();

    CodeGen::emit("mov rdi, rax");

    Type *member_type = static_cast<VariableSymbol*>(member)->getType();
    
    if ( member_type->isReference() )
    {
	CodeGen::emit("mov rax, [rax - " + std::to_string(static_cast<StructSymbol*>(base_type)->getAddress(static_cast<VariableSymbol*>(member))) + "]");
    }
    else if ( member_type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
    {
	OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(member_type);

	auto ov_func_type_info = ov_func->getTypeInfo();

	if ( ov_func_type_info.overloads.size() > 1 )
	{
	    Type *hint_type = GlobalHelper::getTypeHint(this);
	    if ( hint_type == nullptr )
		throw SemanticError("multiple overloads of " + base_type->getName() + "::" + member->getName());

	    member = dynamic_cast<VariableSymbol*>(ov_func_type_info.symbols[static_cast<FunctionSymbol*>(hint_type)->getTypeInfo()]);
	}
	else
	{
	    member = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols.begin()->second);
	}

	CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(static_cast<VariableSymbol*>(member)->getType())->getScopedTypedName() + "]");
    }
    else
    {
	CodeGen::emit("lea rax, [rax - " + std::to_string(static_cast<StructSymbol*>(base_type)->getAddress(static_cast<VariableSymbol*>(member))) + "]");
    }
}

Type* DotNode::getType() const { return TypeHelper::getReferenceType(static_cast<VariableSymbol*>(member)->getType()); }
bool DotNode::isLeftValue() const { return true; }
