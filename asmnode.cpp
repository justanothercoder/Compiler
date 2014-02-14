#include "asmnode.hpp"

AsmNode::AsmNode(string code) : code(code) { }

void AsmNode::build_scope() { }

void AsmNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

void AsmNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

void AsmNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { CodeGen::emit(code); }
