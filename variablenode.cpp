#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "structsymbol.hpp"

VariableNode::VariableNode(string name) : name(name), variable(nullptr) { }

void VariableNode::check()
{
	if ( template_info -> sym && template_info -> sym -> isIn(name) )
		return;

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
	if ( template_info -> sym && template_info -> sym -> isIn(name) )
	{
		auto replace = template_info -> getReplacement(name);
	
		if ( dynamic_cast<VariableNode*>(replace) != nullptr )
			if ( dynamic_cast<VariableNode*>(replace) -> variable -> getSymbolType() == SymbolType::CLASSVARIABLE )
				throw SemanticError(name + " is typename");

		return replace -> gen();
	}

	auto var_type = variable -> getType();

	if ( var_type.type -> getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto function = static_cast<OverloadedFunctionSymbol*>(var_type.type);

		auto function_info = function -> getTypeInfo();

		if ( function_info.overloads.size() > 1 )
		{
			auto hint_type = static_cast<FunctionSymbol*>(this -> type_hint);
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of '" + name + "'.");

			auto type_info = hint_type -> function_type_info;

			if ( function_info.symbols.find(type_info) == std::end(function_info.symbols) )
			{
				auto sym = function -> getViableOverload(type_info);
				variable = new VariableSymbol(function -> getName(), VariableType(sym));
			}
			else
				variable = new VariableSymbol(function -> getName(), VariableType(function_info.symbols[type_info]));
		}
		else
			variable = new VariableSymbol(function -> getName(), VariableType(std::begin(function_info.symbols) -> second));

		code_obj.emit("lea rax, [" + static_cast<FunctionSymbol*>(variable -> getType().type) -> getScopedTypedName() + "]");
	}
	else
	{
		if ( variable -> isField() )
		{
			auto _this = scope -> resolve("this");

			auto sym = static_cast<VariableSymbol*>(_this);

			auto struc_scope = static_cast<StructSymbol*>(sym -> getType().type);

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

bool VariableNode::isTemplateParam() const { return template_info -> sym != nullptr; }
	
AST* VariableNode::copyTree() const { return new VariableNode(name); }

VariableType VariableNode::getType() const
{
	if ( isTemplateParam() )
	{
		auto replace = template_info -> getReplacement(name);
		return replace -> getType();
	}

	return variable -> getType();
}

bool VariableNode::isLeftValue() const { return true; }

void VariableNode::freeTempSpace()
{

}
