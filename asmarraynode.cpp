#include "asmarraynode.hpp"
#include "templateinfo.hpp"
#include "functionsymbol.hpp"
#include "typefactory.hpp"
#include "templatesymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"

AsmArrayNode::AsmArrayNode() : size_of_type(0), array_size(0) 
{ 

}

AST* AsmArrayNode::copyTree() const 
{
   	return new AsmArrayNode(*this); 
}
	
std::string AsmArrayNode::toString() const
{
	return "";
}

void AsmArrayNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
