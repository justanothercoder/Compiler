#include "importnode.hpp"
#include "filehelper.hpp"

ImportNode::ImportNode(std::string lib) : lib(lib) 
{

}

AST* ImportNode::copyTree() const 
{
   	return new ImportNode(lib); 
}

void ImportNode::define() 
{
	auto root = FileHelper::parse((lib + ".txt").c_str());

	root -> scope = scope;
	root -> build_scope();

	root -> define();
	root -> check();
	code_obj.emit(root -> gen().getCode());
}

void ImportNode::check() 
{

}

CodeObject& ImportNode::gen()
{
   	return code_obj; 
}
	
std::string ImportNode::toString() const 
{
	return "import " + lib + ";";
}
