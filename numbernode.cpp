#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num) { }

void NumberNode::build_scope() { }

void NumberNode::check()
{    

}

void NumberNode::gen()
{
    CodeGen::emit("mov qword [rsp - " + std::to_string(sizeof(int*) + getType()->getSize()) + "], " + num);
    CodeGen::emit("lea rax, [rsp - " + std::to_string(sizeof(int*) + getType()->getSize()) + "]");
}

Type* NumberNode::getType() const { return BuiltIns::int_type; }

bool NumberNode::isLeftValue() const { return false; }
