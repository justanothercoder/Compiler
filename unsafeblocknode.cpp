#include "unsafeblocknode.hpp"
#include "unsafescope.hpp"

UnsafeBlockNode::UnsafeBlockNode(StatementNode *block) : block(block)
{
	
}

UnsafeBlockNode::~UnsafeBlockNode()
{
	delete block;
}
	
AST* UnsafeBlockNode::copyTree() const 
{
	return new UnsafeBlockNode(static_cast<StatementNode*>(block -> copyTree()));
}
    
void UnsafeBlockNode::build_scope() 
{
	block -> scope = new UnsafeScope(scope);
	block -> build_scope();
}

void UnsafeBlockNode::define() 
{
	block -> define();
}

void UnsafeBlockNode::check() 
{
	block -> check();
}

CodeObject& UnsafeBlockNode::gen()
{
	return block -> gen();
}

std::vector<AST*> UnsafeBlockNode::getChildren() const 
{
	return block -> getChildren();
}
