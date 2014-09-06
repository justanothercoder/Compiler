#include "variabledeclarationnode.hpp"
#include "classvariablesymbol.hpp"
#include "typehelper.hpp"
#include "callhelper.hpp"
#include "exprnode.hpp"
#include "scope.hpp"
#include "templatestructsymbol.hpp"
#include "builtins.hpp"

VariableDeclarationNode::VariableDeclarationNode(std::string name
		                                       , TypeInfo type_info
											   , bool is_field
											   , std::vector<ExprNode*> constructor_call_params) : name(name)
																								 , type_info(type_info)
																								 , is_field(is_field)
																								 , constructor_call_params(constructor_call_params)
{
    definedSymbol = new VariableSymbol(name, nullptr, (is_field ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE));
}

VariableDeclarationNode::~VariableDeclarationNode() 
{ 
	delete definedSymbol; 

	for ( auto i : constructor_call_params )
		delete i;
}

void VariableDeclarationNode::build_scope()
{
	AST::build_scope();
	for ( auto param : type_info.template_params )
	{
		param -> scope         = scope;
		param -> build_scope();
	}
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const 
{ 
	return definedSymbol; 
}

void VariableDeclarationNode::check()
{	
	for ( auto param : type_info.template_params )
		param -> check();

	if ( !is_field )
	{
		if ( !type_info.is_ref )
		{
			std::string type_name = type_info.type_name;

			auto _ = TypeHelper::fromTypeInfo(type_info, scope, scope -> getTemplateInfo());

			if ( _ -> getSymbol() == nullptr || _ -> getSymbol() -> getSymbolType() != SymbolType::STRUCT )
				throw SemanticError("No such struct " + type_name);

			auto struct_symbol = static_cast<const StructSymbol*>(_ -> getSymbol());
			call_info = CallHelper::callCheck(struct_symbol -> getName(), struct_symbol, constructor_call_params);
		}
		else
		{
			for ( auto param : constructor_call_params )
				param -> check();
		}
	}
	
	getDefinedSymbol() -> is_defined = true;
}

CodeObject& VariableDeclarationNode::gen()
{
	if ( !is_field )
	{  
		if ( type_info.is_ref )
		{
			for ( auto i : constructor_call_params )
				i -> gen();

			code_obj.emit("mov [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(definedSymbol)) + "], rax");
		}
		else
		{
			CodeObject var_code;
			var_code.emit("lea rax, [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(definedSymbol)) + "]");

			code_obj.genCallCode(call_info, constructor_call_params, var_code, false);
		}
	}

	return code_obj;
}

void VariableDeclarationNode::define()
{
	const auto& template_info = scope -> getTemplateInfo();

    if ( template_info.sym && template_info.sym -> isIn(type_info.type_name) )
    {
		auto replace = template_info.getReplacement(type_info.type_name);

		type_info.type_name = boost::get<std::string>(*replace);
    }
    
    auto var_type = TypeHelper::fromTypeInfo(type_info, scope, template_info);
    
    if ( var_type == BuiltIns::void_type )
		throw SemanticError("can't declare a variable of 'void' type.");
   
    definedSymbol -> setType(var_type);
	scope -> define(definedSymbol);
}

AST* VariableDeclarationNode::copyTree() const
{
	std::vector<ExprNode*> params(constructor_call_params.size());

    std::transform(std::begin(constructor_call_params), std::end(constructor_call_params), std::begin(params), [&] (ExprNode *expr) { return static_cast<ExprNode*>(expr -> copyTree()); });
    
    return new VariableDeclarationNode(name, type_info, is_field, params);
}

std::vector<AST*> VariableDeclarationNode::getChildren() const 
{ 
	return std::vector<AST*>(std::begin(constructor_call_params), std::end(constructor_call_params)); 
}
