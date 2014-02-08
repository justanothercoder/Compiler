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

void NumberNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr) { }

void NumberNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}

AST* NumberNode::copyTree() const
{
    return new NumberNode(num);
}

void NumberNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
