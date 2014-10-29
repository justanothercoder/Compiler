#include "functiondeclarationnode.hpp"

#include "typefactory.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(std::string name
		                                       , std::vector< pair<std::string, TypeInfo> > params
											   , TypeInfo return_type_info
											   , AST *statements
											   , FunctionTraits traits
											   , bool is_unsafe) : name(name)
																 , params(params)
																 , return_type_info(return_type_info)
																 , statements(statements)
																 , traits(traits)
																 , definedSymbol(nullptr) 
																 , is_unsafe(is_unsafe)
{

}

void FunctionDeclarationNode::build_scope()
{
	definedSymbol = new FunctionSymbol(traits.is_constructor ? static_cast<StructSymbol*>(scope) -> getName() : name, nullptr, { }, scope, traits);
	definedSymbol -> is_unsafe = is_unsafe;

	statements -> scope = definedSymbol;
	statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

CodeObject& FunctionDeclarationNode::gen()
{    
	std::string scoped_typed_name = definedSymbol -> getScopedTypedName();

	code_obj.emit("jmp _~" + scoped_typed_name);
	code_obj.emit(scoped_typed_name + ":");
	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");

	if ( definedSymbol -> getVarAlloc().getSpace() > 0 )
		code_obj.emit("sub rsp, " + std::to_string(definedSymbol -> getVarAlloc().getSpace()));
	code_obj.emit("sub rsp, " + std::to_string(definedSymbol -> getTempAlloc().getSpaceNeeded()));

	code_obj.emit(statements -> gen().getCode());

	if ( definedSymbol -> isConstructor() )
		code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

//	code_obj.emit("add rsp, " + std::to_string(scope -> getTempAlloc().getSpaceNeeded()));
	
	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");
	code_obj.emit("_~" + scoped_typed_name + ":");

	definedSymbol -> code_obj = code_obj;

	return code_obj;
}

AST* FunctionDeclarationNode::copyTree() const 
{ 
	return new FunctionDeclarationNode(name, params, return_type_info, statements -> copyTree(), traits); 
}

std::vector<AST*> FunctionDeclarationNode::getChildren() const 
{ 
	return {statements}; 
}
	
std::string FunctionDeclarationNode::toString() const
{
	std::string res = return_type_info.toString() + " " + name + "(";
	
	if ( !params.empty() )
	{
		auto it = std::begin(params);

		res += it -> second.toString() + " " + it -> first;

		for ( ++it; it != std::end(params); ++it )
			res += ", " + it -> second.toString() + " " + it -> first;
	}

	res += ")";
	res += statements -> toString();

	return res;
}

void FunctionDeclarationNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
