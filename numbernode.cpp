#include "numbernode.hpp"

NumberNode::NumberNode(string num) : num(num) { }

void NumberNode::build_scope() { }

void NumberNode::check() { }

void NumberNode::gen()
{
    CodeGen::emit("mov qword [rsp - " + std::to_string(GlobalConfig::int_size + getType()->getSize()) + "], " + num);
    CodeGen::emit("lea rax, [rsp - " + std::to_string(GlobalConfig::int_size + getType()->getSize()) + "]");
}

Type* NumberNode::getType() const { return BuiltIns::int_type; }

bool NumberNode::isLeftValue() const { return false; }

void NumberNode::template_check(TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr) { }
bool NumberNode::isTemplated() const { return false; }
