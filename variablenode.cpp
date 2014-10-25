#include "variablenode.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "builtins.hpp"
#include "globalhelper.hpp"

VariableNode::VariableNode(std::string name) : name(name), variable(nullptr), template_num(nullptr) 
{

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
	else
		return false;
}

boost::optional<int> VariableNode::getCompileTimeValue() const
{
	const auto& template_info = scope -> getTemplateInfo();

	if ( template_info.sym != nullptr && template_info.sym -> isIn(name) )
		return boost::get<int>(*template_info.getReplacement(name));
	else
		return boost::none;
}

std::string VariableNode::toString() const
{
	return name;
}
	
void VariableNode::accept(ASTVisitor& visitor)
{
	visitor.visit(this);
}
	
int VariableNode::getVarId() const
{
	return GlobalHelper::id_by_var[variable];
}
