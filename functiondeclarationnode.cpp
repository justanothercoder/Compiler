#include "functiondeclarationnode.hpp"

#include "typefactory.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name
		                                       , vector< pair<string, TypeInfo> > params
											   , TypeInfo return_type_info
											   , AST *statements
											   , FunctionTraits traits) : name(name)
																		, params(params)
																		, return_type_info(return_type_info)
																		, statements(statements)
																		, traits(traits)
																		, definedSymbol(nullptr) 
{

}

FunctionDeclarationNode::~FunctionDeclarationNode() 
{ 
	delete definedSymbol; 
	delete statements;	
}   

void FunctionDeclarationNode::build_scope()
{
	definedSymbol = new FunctionSymbol(traits.is_constructor ? static_cast<StructSymbol*>(scope) -> getName() : name, nullptr, { }, scope, traits);

	statements -> scope         = definedSymbol;
	statements -> build_scope();
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void FunctionDeclarationNode::define()
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && return_type_info.type_name == template_info.sym -> getName() )
		return_type_info.type_name = static_cast<StructSymbol*>(scope) -> getName();

	auto fromTypeInfo = [&] (TypeInfo type_info) -> const Type*
	{
		if ( template_info.sym != nullptr && type_info.type_name == template_info.sym -> getName() )
			type_info.type_name = static_cast<StructSymbol*>(scope) -> getName();

		if ( definedSymbol -> isMethod() && type_info.type_name == static_cast<StructSymbol*>(scope) -> getName() )
		{
			const Type *type = static_cast<const StructSymbol*>(scope);

			if ( type_info.is_ref )
				type = TypeFactory::getReference(type);

			if ( type_info.is_const )
				type = TypeFactory::getConst(type);

			return type;
		}
		return TypeHelper::fromTypeInfo(type_info, scope, template_info);
	};

    auto return_type = fromTypeInfo(return_type_info);

	vector<const Type*> params_types;

	if ( traits.is_method )
	{
		auto _this_type = TypeFactory::getReference(static_cast<StructSymbol*>(scope));
		params_types.push_back(_this_type);
		
		auto _this_sym = new VariableSymbol("this", _this_type, VariableSymbolType::PARAM);

		params_symbols.push_back(_this_sym);
		definedSymbol -> accept(new VariableSymbolDefine(_this_sym));
	}

	for ( auto i : params )
	{
		auto param_type = fromTypeInfo(i.second);

		params_types.push_back(param_type);

		auto param_sym = new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM);

		params_symbols.push_back(param_sym);
		definedSymbol -> accept(new VariableSymbolDefine(param_sym));
	}

	FunctionTypeInfo function_type_info(params_types);

	definedSymbol -> return_type = return_type;
	definedSymbol -> function_type_info = function_type_info;

	scope -> accept(new FunctionSymbolDefine(definedSymbol));

	statements -> define();
}

CodeObject& FunctionDeclarationNode::gen()
{    
	string scoped_typed_name = definedSymbol -> getScopedTypedName();

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

void FunctionDeclarationNode::check() 
{ 
	scope -> resolve(name) -> is_defined = true;

	for ( auto param : params_symbols )
		param -> is_defined = true;

	statements -> check(); 
}

AST* FunctionDeclarationNode::copyTree() const 
{ 
	return new FunctionDeclarationNode(name, params, return_type_info, statements -> copyTree(), traits); 
}

vector<AST*> FunctionDeclarationNode::getChildren() const { return {statements}; }
