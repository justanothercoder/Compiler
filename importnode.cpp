#include "importnode.hpp"

ImportNode::ImportNode(string lib) : lib(lib) { }

AST* ImportNode::copyTree() const { return new ImportNode(lib); }

void ImportNode::define(const TemplateInfo&) 
{
	auto root = FileHelper::parse((lib + ".txt").c_str());

	root->build_scope();

	root->define(TemplateInfo());
	root->check(TemplateInfo());
	root->gen(TemplateInfo());
}

void ImportNode::check(const TemplateInfo&) { }
void ImportNode::gen(const TemplateInfo&) { }

vector<AST*> ImportNode::getChildren() const { return { }; }
