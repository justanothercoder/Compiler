#include "variabledeclarationnode.hpp"
#include "classvariablesymbol.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, TypeInfo type_info, bool is_field, const vector<ExprNode*>& constructor_call_params) : name(name),
																		     type_info(type_info),
																		     is_field(is_field),
																		     constructor_call_params(constructor_call_params),
																			 call_info() 
{
    definedSymbol = new VariableSymbol(name, VariableType(), (is_field ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE));
}

VariableDeclarationNode::~VariableDeclarationNode() 
{ 
	delete definedSymbol; 

	for ( auto i : constructor_call_params )
		delete i;
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void VariableDeclarationNode::check()
{
	if ( !is_field )
	{
		if ( !type_info.is_ref )
		{
			string type_name = type_info.type_name;

			auto _ = TypeHelper::fromTypeInfo(type_info, scope, template_info);

			if ( _.type -> getTypeKind() != TypeKind::STRUCT )
				throw SemanticError("No such struct " + type_name);

			auto type = static_cast<StructSymbol*>(_.type);
			call_info = CallHelper::callCheck(type_name, type, constructor_call_params);
		}
		else
		{
			for ( auto i : constructor_call_params )
				i -> check();
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
    if ( template_info -> sym && template_info -> sym -> isIn(type_info.type_name) )
    {
		auto replace = template_info -> getReplacement(type_info.type_name);

		auto sym = replace -> getType();
		
		type_info.type_name = sym.getTypeName();
    }
    
    auto var_type = TypeHelper::fromTypeInfo(type_info, scope, template_info);
    
    if ( var_type.type == BuiltIns::void_type )
		throw SemanticError("can't declare a variable of 'void' type.");
   
    definedSymbol -> setType(var_type);
	scope -> accept(new VariableSymbolDefine(definedSymbol));
}

AST* VariableDeclarationNode::copyTree() const
{
    vector<ExprNode*> params(constructor_call_params.size());

    std::transform(std::begin(constructor_call_params), std::end(constructor_call_params), std::begin(params), [&] (ExprNode *expr) { return static_cast<ExprNode*>(expr -> copyTree()); });
    
    return new VariableDeclarationNode(name, type_info, is_field, params);
}

vector<AST*> VariableDeclarationNode::getChildren() const 
{ 
	return vector<AST*>(std::begin(constructor_call_params), std::end(constructor_call_params)); 
}
