#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, AST* statements, FunctionTraits traits) : name(name), params(params), return_type_info(return_type_info), statements(statements), traits(traits), definedSymbol(nullptr) { }

FunctionDeclarationNode::~FunctionDeclarationNode() 
{ 
	delete definedSymbol; 
	delete statements;	
}   

void FunctionDeclarationNode::build_scope()
{
	definedSymbol = new FunctionSymbol(name, FunctionTypeInfo(VariableType(), { }), getScope(), traits);

	statements->setScope(definedSymbol);
	statements->build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void FunctionDeclarationNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym != nullptr && return_type_info.type_name == template_sym->getName() )
		return_type_info.type_name = static_cast<StructSymbol*>(getScope())->getName();

	VariableType return_type;
   	if ( definedSymbol->isMethod() && return_type_info.type_name == static_cast<StructSymbol*>(getScope())->getName() )
		return_type.type = static_cast<StructSymbol*>(getScope());
	else
		return_type = TypeHelper::fromTypeInfo(return_type_info, getScope());

	vector<VariableType> params_types;

	if ( traits.is_method )
	{
		auto _this_type = VariableType(static_cast<StructSymbol*>(getScope()), true, false);
		params_types.push_back(_this_type);

		definedSymbol->accept(new VariableSymbolDefine(new VariableSymbol("this", _this_type, VariableSymbolType::PARAM)));
	}

	for ( auto i : params )
	{
		VariableType param_type;
   		if ( definedSymbol->isMethod() && i.second.type_name == static_cast<StructSymbol*>(getScope())->getName() )
		{			
			param_type.type = static_cast<StructSymbol*>(getScope());

			param_type.is_ref = i.second.is_ref;
			param_type.is_const = i.second.is_const;
		}
		else
			param_type = TypeHelper::fromTypeInfo(i.second, getScope());

		params_types.push_back(param_type);

		definedSymbol->accept(new VariableSymbolDefine(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM)));
	}

	FunctionTypeInfo function_type_info(return_type, params_types);

	definedSymbol->setTypeInfo(function_type_info);

	getScope()->accept(new FunctionSymbolDefine(definedSymbol));

	statements->define(template_sym, expr);
}

void FunctionDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
	string scoped_typed_name = definedSymbol->getScopedTypedName();

	CodeGen::emit("jmp _~" + scoped_typed_name);
	CodeGen::emit(scoped_typed_name + ":");
	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");

	if ( definedSymbol->get_valloc()->getSpace() > 0 )
		CodeGen::emit("sub rsp, " + std::to_string(definedSymbol->get_valloc()->getSpace()));

	statements->gen(template_sym, expr);

	if ( definedSymbol->isConstructor() )
		CodeGen::emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");
	CodeGen::emit("_~" + scoped_typed_name + ":");
}

void FunctionDeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	statements->check(template_sym, expr);
}

AST* FunctionDeclarationNode::copyTree() const { return new FunctionDeclarationNode(name, params, return_type_info, statements->copyTree(), traits); }

vector<AST*> FunctionDeclarationNode::getChildren() const { return {statements}; }
