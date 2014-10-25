#include "varinfertypedeclarationnode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "callhelper.hpp"
#include "variablesymboldefine.hpp"
#include "globalhelper.hpp"
#include "exprnode.hpp"

VarInferTypeDeclarationNode::VarInferTypeDeclarationNode(std::string name, ExprNode *expr) : name(name), expr(expr), definedSymbol(nullptr)
{

}

Symbol* VarInferTypeDeclarationNode::getDefinedSymbol() const
{
	return definedSymbol;
}

CodeObject& VarInferTypeDeclarationNode::gen() 
{
	auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
	scope -> getTempAlloc().claim(expr -> getType() -> getSize());

	CodeObject var_code;
	var_code.emit("lea rax, [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(definedSymbol)) + "]");

	code_obj.genCallCode(call_info, {expr}, var_code, false);

	return code_obj;
}

AST* VarInferTypeDeclarationNode::copyTree() const
{
	return new VarInferTypeDeclarationNode(name, static_cast<ExprNode*>(expr -> copyTree()));
}

std::vector<AST*> VarInferTypeDeclarationNode::getChildren() const
{
	return {expr};
}
	
std::string VarInferTypeDeclarationNode::toString() const 
{
	return "var " + name + " = " + expr -> toString() + ";";
}

void VarInferTypeDeclarationNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
