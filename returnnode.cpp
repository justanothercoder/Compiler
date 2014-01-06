#include "returnnode.hpp"

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr)
{

}

void ReturnNode::build_scope()
{
    expr->scope = scope;
    expr->build_scope();
}

void ReturnNode::define()
{
    
}

void ReturnNode::check()
{
    Scope *sc = scope;
    while ( sc != nullptr && dynamic_cast<FunctionSymbol*>(sc) == nullptr )
	sc = sc->getEnclosingScope();

    if ( sc == nullptr )
	throw SemanticError("return is not a in a function");

    func = dynamic_cast<FunctionSymbol*>(sc);

    expr->check();    
}

void ReturnNode::gen()
{
    expr->gen();
    for ( int i = 0; i < expr->getType()->getSize(); i += sizeof(int*) )
    {
	CodeGen::emit("mov rbx, [rax - " + std::to_string(i) +  "]");
	CodeGen::emit("mov [rsp - " + std::to_string(i + sizeof(int*)) +  "], rbx");
    }
    CodeGen::emit("lea rax, [rsp - " + std::to_string(sizeof(int*)) + "]");

    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
}
