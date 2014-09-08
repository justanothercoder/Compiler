#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "classvariablesymbol.hpp"
#include "builtins.hpp"

VariableNode::VariableNode(std::string name) : name(name), variable(nullptr), template_num(nullptr) 
{

}

void VariableNode::check()
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym && template_info.sym -> isIn(name) )
	{
		auto replace = template_info.getReplacement(name);

		if ( replace -> which() == 0 )
			throw SemanticError(name + " is typename");

		template_num = new NumberNode(std::to_string(boost::get<int>(*replace)));
		template_num -> scope = scope;
		template_num -> build_scope();

		template_num -> check();
		return;
	}

	auto sym = scope -> resolve(name);

	if ( sym == nullptr || !sym -> is_defined )
		throw SemanticError("No such symbol '" + name + "'.");

	if ( sym -> getSymbolType() == SymbolType::STRUCT )
	{
		variable = new ClassVariableSymbol(static_cast<StructSymbol*>(sym));
		return;
	}
	else
	{
		if ( sym -> getSymbolType() != SymbolType::VARIABLE )
			throw SemanticError("'" + name + "' is not a variable.");
	}

	variable = static_cast<VariableSymbol*>(sym);
}

CodeObject& VariableNode::gen()
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym && template_info.sym -> isIn(name) )
		return template_num -> gen();

	auto var_type = variable -> getType();

	if ( var_type -> getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto function = static_cast<const OverloadedFunctionSymbol*>(var_type);

		auto function_info = function -> getTypeInfo();

		if ( function_info.overloads.size() > 1 )
		{
			auto hint_type = static_cast<const FunctionSymbol*>(this -> type_hint);
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of '" + name + "'.");

			auto type_info = hint_type -> function_type_info;

			if ( function_info.symbols.find(type_info) == std::end(function_info.symbols) )
			{
				auto sym = function -> getViableOverload(type_info);
				variable = new VariableSymbol(function -> getName(), sym);
			}
			else
				variable = new VariableSymbol(function -> getName(), function_info.symbols[type_info]);
		}
		else
			variable = new VariableSymbol(function -> getName(), std::begin(function_info.symbols) -> second);

		code_obj.emit("lea rax, [" + static_cast<const FunctionSymbol*>(variable -> getType()) -> getScopedTypedName() + "]");
	}
	else
	{
		if ( variable -> isField() )
		{
			auto _this = scope -> resolve("this");

			auto sym = static_cast<VariableSymbol*>(_this);

			auto struc_scope = static_cast<const StructSymbol*>(sym -> getType() -> getSymbol());

			code_obj.emit("mov rax, [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(sym)) + "]");
			code_obj.emit("lea rax, [rax - " + std::to_string(struc_scope -> getVarAlloc().getAddress(variable)) + "]");
		}
		else
		{
			code_obj.emit("lea rax, [rbp - " + std::to_string(scope -> getVarAlloc().getAddress(variable)) + "]");
		}
	}

	return code_obj;
}

bool VariableNode::isTemplateParam() const 
{ 
	const auto& template_info = scope -> getTemplateInfo();
	return template_info.sym != nullptr && template_info.sym -> isIn(name); 
}
	
AST* VariableNode::copyTree() const { return new VariableNode(name); }

const Type* VariableNode::getType() const
{
	if ( isTemplateParam() )
	{
		const auto& template_info = scope -> getTemplateInfo();

		auto replace = template_info.getReplacement(name);

		return BuiltIns::global_scope -> resolveType("int");
	}

	return variable -> getType();
}

bool VariableNode::isLeftValue() const 
{
   	return true; 
}

void VariableNode::freeTempSpace()
{

}

bool VariableNode::isCompileTimeExpr() const
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && template_info.sym -> isIn(name) )
		return true;
	else if ( dynamic_cast<ClassVariableSymbol*>(variable) )
		return true;
	else
		return false;
}

boost::optional<int> VariableNode::getCompileTimeValue() const
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && template_info.sym -> isIn(name) )
		return boost::get<int>(*template_info.getReplacement(name));
	if ( dynamic_cast<ClassVariableSymbol*>(variable) )
		return std::hash<std::string>()(variable -> getName());
	else
		return boost::none;
}
