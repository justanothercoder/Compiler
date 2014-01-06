#include "dotnode.hpp"

DotNode::DotNode(ExprNode *base, string member_name) : base(base), member_name(member_name)
{
    
}

void DotNode::build_scope()
{
    base->scope = scope;
    base->build_scope();    
}

void DotNode::check()
{
    base->check();

    StructSymbol * struc = dynamic_cast<StructSymbol*>(base->getType());

    if ( struc == nullptr )
	throw SemanticError("left side of '.' is not a struct instance.");

    member = struc->resolveMember(member_name);

    if ( member == nullptr )
	throw SemanticError(member_name + " is not member of " + struc->getName());
}

void DotNode::gen()
{
    base->gen();

    CodeGen::emit("lea rax, [rax - " + std::to_string(static_cast<StructSymbol*>(base->getType())->getAddress(static_cast<VariableSymbol*>(member))) + "]");
}

Type* DotNode::getType()
{
    return static_cast<VariableSymbol*>(member)->getType();
}

bool DotNode::isLeftValue()
{
    return true;
}
