#include "structdeclarationnode.hpp"
#include "structsymbol.hpp"
#include "symboldefine.hpp"
#include "functionhelper.hpp"
#include "functionsymbol.hpp"

StructDeclarationNode::StructDeclarationNode(std::string name, std::vector<AST*> inner, boost::optional<TemplateInfo> template_info) 
																	: name(name)
																	, inner(inner)
																	, template_info(template_info)
																	, definedSymbol(nullptr)
{

}

StructDeclarationNode::~StructDeclarationNode() 
{ 
	delete definedSymbol; 

	for ( auto i : inner )
		delete i;
}

Symbol* StructDeclarationNode::getDefinedSymbol() const 
{ 
	return definedSymbol; 
}

void StructDeclarationNode::build_scope()
{
	if ( !template_info )
	    definedSymbol = new StructSymbol(name, scope, scope -> getTemplateInfo());
	else
	    definedSymbol = new StructSymbol(name, scope, *template_info);

	scope -> define(definedSymbol);

	for ( auto i : inner )
    {
		i -> scope = definedSymbol;
		i -> build_scope();
    }
}

void StructDeclarationNode::define()
{
    for ( auto decl : inner )
		decl -> define();
}

void StructDeclarationNode::check()
{
	getDefinedSymbol() -> is_defined = true;
	
	if ( definedSymbol -> getDefaultConstructor() == nullptr )
	{
		auto default_constr = FunctionHelper::makeDefaultConstructor(definedSymbol);
		definedSymbol -> define(default_constr);
		default_constr_code = *default_constr->code_obj;
	}

	if ( definedSymbol -> getCopyConstructor() == nullptr )
	{
		auto copy_constr = FunctionHelper::makeDefaultCopyConstructor(definedSymbol);
		definedSymbol -> define(copy_constr);
		default_copy_constr_code = *copy_constr->code_obj;
	}

	for ( auto decl : inner )
	{
		if ( dynamic_cast<DeclarationNode*>(decl) )
			static_cast<DeclarationNode*>(decl) -> getDefinedSymbol() -> is_defined = true;
	}

	for ( auto decl : inner )
		decl->check();	
}

CodeObject& StructDeclarationNode::gen()
{
	code_obj.emit(default_constr_code.getCode());
	code_obj.emit(default_copy_constr_code.getCode());
    for ( auto decl : inner )
		code_obj.emit(decl -> gen().getCode());
	return code_obj;
}

AST* StructDeclarationNode::copyTree() const 
{ 
	std::vector<AST*> in(inner.size());
	std::transform(std::begin(inner), std::end(inner), std::begin(in), [&](AST *t) { return t -> copyTree(); });
	return new StructDeclarationNode(name, in, template_info);
}

std::vector<AST*> StructDeclarationNode::getChildren() const 
{
   	return inner; 
}	

std::string StructDeclarationNode::toString() const
{
	std::string res = "struct " + name + "\n{\n";
	
	for ( auto decl : inner )
		res += decl -> toString() + '\n';

	res += "}";
	return res;
}

void StructDeclarationNode::accept(ASTVisitor *visitor)
{
	visitor -> visit(this);
}
