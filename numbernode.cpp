#include "numbernode.hpp"

Type* NumberNode::expr_type = nullptr;

NumberNode::NumberNode(string num) : num(num) { }

void NumberNode::build_scope() { }

void NumberNode::check()
{    
    if ( expr_type == nullptr )
	expr_type = dynamic_cast<Type*>(scope->resolve("int"));
}

void NumberNode::gen()
{
    CodeGen::emit("mov qword [rsp - " + std::to_string(getType()->getSize()) + "], " + num);
    CodeGen::emit("lea rax, [rsp - " + std::to_string(getType()->getSize()) + "]");
}

Type* NumberNode::getType() const { return expr_type; }

bool NumberNode::isLeftValue() const { return false; }
