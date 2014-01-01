#include "numbernode.hpp"

Type* NumberNode::expr_type = nullptr;

NumberNode::NumberNode(string num) : num(num)
{
    
}

void NumberNode::check()
{    
    if ( expr_type == nullptr )
    {
	expr_type = dynamic_cast<Type*>(scope->resolve("int"));

	if ( expr_type == nullptr )
	    throw;
    }
}

Type* NumberNode::getType()
{
    return expr_type;
}

void NumberNode::gen()
{
    CodeGen::emit("mov qword [rsp - " + std::to_string(getType()->getSize()) + "], " + num);
    CodeGen::emit("lea rax, [rsp - " + std::to_string(getType()->getSize()) + "]");
}

bool NumberNode::isLeftValue()
{
    return false;
}

void NumberNode::build_scope()
{
    
}
