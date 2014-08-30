#include "varinfertypedeclarationnode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "callhelper.hpp"
#include "variablesymboldefine.hpp"
#include "globalhelper.hpp"

VarInferTypeDeclarationNode::VarInferTypeDeclarationNode(std::string name, ExprNode *expr) : name(name), expr(expr), definedSymbol(nullptr)
{

}

Symbol* VarInferTypeDeclarationNode::getDefinedSymbol() const
{
	return definedSymbol;
}

void VarInferTypeDeclarationNode::define() 
{
	expr -> check();

	if ( scope -> resolve(name) != nullptr )
		throw SemanticError(name + " is already defined");

	if ( expr -> getType() == BuiltIns::void_type )
		throw SemanticError("can't define variable of 'void' type");

	definedSymbol = new VariableSymbol(name, expr -> getType());

	scope -> accept(new VariableSymbolDefine(definedSymbol));
}
	
void VarInferTypeDeclarationNode::check() 
{
	scope -> getTempAlloc().add(expr -> getType() -> getSize());	
	getDefinedSymbol() -> is_defined = true;

	auto type = expr -> getType();

	call_info = CallHelper::callCheck(type -> getName(), static_cast<const StructSymbol*>(type), {expr});
}

CodeObject& VarInferTypeDeclarationNode::gen() 
{
	string addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
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
