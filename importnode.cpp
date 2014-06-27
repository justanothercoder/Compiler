#include "importnode.hpp"

ImportNode::ImportNode(string lib) : lib(lib), code_obj() { }

AST* ImportNode::copyTree() const { return new ImportNode(lib); }

void ImportNode::define() 
{
	auto root = FileHelper::parse((lib + ".txt").c_str());

	root -> scope = scope;
	root -> template_info = new TemplateInfo();
	root -> build_scope();

	root -> define();
	root -> check();
	code_obj.emit(root -> gen().getCode());
}

void ImportNode::check() { }

CodeObject& ImportNode::gen() { return code_obj; }

int ImportNode::neededSpaceForTemporaries()
{
	return 0;
}
