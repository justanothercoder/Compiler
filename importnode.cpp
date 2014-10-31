#include "importnode.hpp"
#include "filehelper.hpp"

ImportNode::ImportNode(std::string lib) : lib(lib) 
{

}

AST* ImportNode::copyTree() const 
{
   	return new ImportNode(lib); 
}

std::string ImportNode::toString() const 
{
	return "import " + lib + ";";
}

void ImportNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
