#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, AST* statements, FunctionTraits traits) : name(name), params(params), return_type_info(return_type_info), statements(statements), traits(traits), definedSymbol(nullptr) { }

FunctionDeclarationNode::~FunctionDeclarationNode() 
{ 
	delete definedSymbol; 
	delete statements;	
}   

void FunctionDeclarationNode::build_scope()
{
	definedSymbol = new FunctionSymbol(name, FunctionTypeInfo(nullptr, { }), this->getScope(), traits);

	statements->setScope(definedSymbol);
	statements->build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void FunctionDeclarationNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym != nullptr && return_type_info.getTypeName() == template_sym->getName() )
	{
		return_type_info = TypeInfo(
				static_cast<StructSymbol*>(this->getScope())->getName(),
				return_type_info.getIsRef(),
				return_type_info.getTemplateParams()
				);
	}
	
	Type *return_type = TypeHelper::fromTypeInfo(return_type_info, this->getScope());

	vector<Type*> params_types;

	if ( traits.is_method )
	{
		Type *_this_type = TypeHelper::getReferenceType(static_cast<StructSymbol*>(this->getScope()));
		params_types.push_back(_this_type);

		definedSymbol->accept(new VariableSymbolDefine(new VariableSymbol("this", _this_type, VariableSymbolType::PARAM)));
	}

	for ( auto i : params )
	{
		Type *param_type = TypeHelper::fromTypeInfo(i.second, this->getScope());	
		params_types.push_back(param_type);

		definedSymbol->accept(new VariableSymbolDefine(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM)));
	}

	FunctionTypeInfo function_type_info(return_type, params_types);

	definedSymbol->setTypeInfo(function_type_info);

	this->getScope()->accept(new FunctionSymbolDefine(definedSymbol));

	statements->define(template_sym, expr);
}

void FunctionDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
	string scoped_typed_name = definedSymbol->getScopedTypedName();

	CodeGen::emit("jmp _~" + scoped_typed_name);
	CodeGen::emit(scoped_typed_name + ":");
	CodeGen::emit("push rbp");
	CodeGen::emit("mov rbp, rsp");

	statements->gen(template_sym, expr);

	CodeGen::emit("mov rsp, rbp");
	CodeGen::emit("pop rbp");
	CodeGen::emit("ret");
	CodeGen::emit("_~" + scoped_typed_name + ":");
}

void FunctionDeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	definedSymbol->recalc_scope_address();
	statements->check(template_sym, expr);
}

AST* FunctionDeclarationNode::copyTree() const { return new FunctionDeclarationNode(name, params, return_type_info, statements->copyTree(), traits); }

vector<AST*> FunctionDeclarationNode::getChildren() const { return {statements}; }
