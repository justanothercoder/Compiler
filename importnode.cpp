#include "importnode.hpp"

ImportNode::ImportNode(string lib) : lib(lib), code_obj() { }

AST* ImportNode::copyTree() const { return new ImportNode(lib); }

void ImportNode::define(const TemplateInfo&) 
{
	auto root = FileHelper::parse((lib + ".txt").c_str());

	root->scope = scope;
	root->build_scope();

	root->define(TemplateInfo());
	root->check(TemplateInfo());
	code_obj.emit(root->gen(TemplateInfo()).getCode());
}

void ImportNode::check(const TemplateInfo&) { }
CodeObject& ImportNode::gen(const TemplateInfo&) { return code_obj; }
