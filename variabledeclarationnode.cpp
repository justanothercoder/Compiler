#include "variabledeclarationnode.hpp"
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
		if ( param.which() == 0 )
		{
			boost::get<ExprNode*>(param) -> scope = scope;
			boost::get<ExprNode*>(param) -> build_scope();
		}
	}
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const 
{ 
	return definedSymbol; 
}

void VariableDeclarationNode::check()
{	
	for ( auto param : type_info.template_params )
	{
		if ( param.which() == 0 )
			boost::get<ExprNode*>(param) -> check();
	}

	if ( !is_field )
	{
		if ( !type_info.is_ref )
		{
			std::string type_name = type_info.type_name;

			auto var_type = scope -> fromTypeInfo(type_info);

//			if ( var_type -> getSymbol() == nullptr || var_type -> getSymbol() -> getSymbolType() != SymbolType::STRUCT )
//				throw SemanticError("No such struct '" + type_name + "'");

			if ( var_type -> getTypeKind() != TypeKind::POINTER )
			{
				auto struct_symbol = static_cast<const StructSymbol*>(var_type -> getSymbol());
				call_info = CallHelper::callCheck(struct_symbol -> getName(), struct_symbol, constructor_call_params);
			}
			else
			{
				for ( auto param : constructor_call_params )
					param -> check();
			}
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
				code_obj.emit(i -> gen().getCode());

			code_obj.emit("mov [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(definedSymbol)) + "], rax");
		}
		else if ( type_info.pointer_depth > 0 )
		{
			if ( !constructor_call_params.empty() )
			{
				for ( auto i : constructor_call_params )
					code_obj.emit(i -> gen().getCode());

				code_obj.emit("mov rbx, [rax]");
				code_obj.emit("mov [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(definedSymbol)) + "], rbx");
			}
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

//		type_info.type_name = boost::get<std::string>(*replace);
		type_info = boost::get<TypeInfo>(*replace);
    }
    
    auto var_type = scope -> fromTypeInfo(type_info);
    
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
	
std::string VariableDeclarationNode::toString() const
{
	return type_info.toString() + " " + name + ";";
}

void VariableDeclarationNode::accept(ASTVisitor *visitor)
{
	visitor -> visit(this);
}
