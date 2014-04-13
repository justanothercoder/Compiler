#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, vector< pair<string, TypeInfo> > params, TypeInfo return_type_info, AST* statements, FunctionTraits traits) : name(name), params(params), return_type_info(return_type_info), statements(statements), traits(traits), definedSymbol(nullptr) { }

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

void FunctionDeclarationNode::define(const TemplateInfo& template_info)
{
	if ( template_info.sym != nullptr && return_type_info.type_name == template_info.sym->getName() )
		return_type_info.type_name = static_cast<StructSymbol*>(getScope())->getName();

	auto fromTypeInfo = [&] (TypeInfo type_info) 
	{
		if ( definedSymbol->isMethod() && type_info.type_name == static_cast<StructSymbol*>(getScope())->getName() )
			return VariableType(static_cast<StructSymbol*>(getScope()), type_info.is_ref, type_info.is_const);
		return TypeHelper::fromTypeInfo(type_info, getScope(), template_info);
	};

	VariableType return_type = fromTypeInfo(return_type_info);

	vector<VariableType> params_types;

	if ( traits.is_method )
	{
		auto _this_type = VariableType(static_cast<StructSymbol*>(getScope()), true, false);
		params_types.push_back(_this_type);
		
		auto _this_sym = new VariableSymbol("this", _this_type, VariableSymbolType::PARAM);

		params_symbols.push_back(_this_sym);
		definedSymbol->accept(new VariableSymbolDefine(_this_sym));
	}

	for ( auto i : params )
	{
		VariableType param_type = fromTypeInfo(i.second);

		params_types.push_back(param_type);

		auto param_sym = new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM);

		params_symbols.push_back(param_sym);
		definedSymbol->accept(new VariableSymbolDefine(param_sym));
	}

	FunctionTypeInfo function_type_info(return_type, params_types);

	definedSymbol->setTypeInfo(function_type_info);

	getScope()->accept(new FunctionSymbolDefine(definedSymbol));

	statements->define(template_info);
}

CodeObject& FunctionDeclarationNode::gen(const TemplateInfo& template_info)
{    
	string scoped_typed_name = definedSymbol->getScopedTypedName();

	code_obj.emit("jmp _~" + scoped_typed_name);
	code_obj.emit(scoped_typed_name + ":");
	code_obj.emit("push rbp");
	code_obj.emit("mov rbp, rsp");

	if ( definedSymbol->get_valloc()->getSpace() > 0 )
		code_obj.emit("sub rsp, " + std::to_string(definedSymbol->get_valloc()->getSpace()));

	code_obj.emit(statements->gen(template_info).getCode());

	if ( definedSymbol->isConstructor() )
		code_obj.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");

	code_obj.emit("mov rsp, rbp");
	code_obj.emit("pop rbp");
	code_obj.emit("ret");
	code_obj.emit("_~" + scoped_typed_name + ":");

	return code_obj;
}

void FunctionDeclarationNode::check(const TemplateInfo& template_info) 
{ 
	GlobalHelper::setDefined(getScope()->resolve(name));

	for ( auto param : params_symbols )
		GlobalHelper::setDefined(param);

	statements->check(template_info); 
}

AST* FunctionDeclarationNode::copyTree() const { return new FunctionDeclarationNode(name, params, return_type_info, statements->copyTree(), traits); }

vector<AST*> FunctionDeclarationNode::getChildren() const { return {statements}; }
