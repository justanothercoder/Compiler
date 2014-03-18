#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name), variable(nullptr), template_sym(nullptr) { }

void VariableNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym && template_sym->isIn(name) )
	{
		this->template_sym = const_cast<TemplateStructSymbol*>(template_sym);
		this->template_expr = expr;
		return;
	}

	auto sym = getScope()->resolve(name);

	if ( sym == nullptr )
		throw SemanticError("No such symbol " + name);

	if ( sym->getSymbolType() == SymbolType::STRUCT )
	{
		variable = new ClassVariableSymbol(static_cast<StructSymbol*>(sym));
		return;
	}
	else
	{
		if ( sym->getSymbolType() != SymbolType::VARIABLE )
			throw SemanticError("'" + name + "' is not a variable.");
	}

	variable = static_cast<VariableSymbol*>(sym);
}

Type* VariableNode::getType() const
{
	if ( isTemplateParam() )
	{
		auto replace = template_sym->getReplacement(name, template_expr);
		return replace->getType();
	}

	return TypeHelper::addReference(variable->getType());
}

bool VariableNode::isTemplateParam() const { return template_sym != nullptr; }

AST* VariableNode::copyTree() const { return new VariableNode(name); }

void VariableNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym && template_sym->isIn(name) )
	{
		auto replace = template_sym->getReplacement(name, expr);
		replace->gen(template_sym, expr);	
		return;
	}

	auto var_type = variable->getType();

	if ( var_type->isReference() )
	{
		if ( variable->isField() )
		{
			auto sym = static_cast<VariableSymbol*>(getScope()->resolve("this"));

			auto struc_scope = static_cast<StructSymbol*>(sym->getType());

			CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(sym)) + "]");
			CodeGen::emit("mov rax, [rax - " + std::to_string(struc_scope->get_valloc()->getAddress(variable)) + "]");
		}
		else
			CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(variable)) + "]");
	}    
	else if ( var_type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(var_type);

		auto ov_func_type_info = ov_func->getTypeInfo();

		if ( ov_func_type_info.overloads.size() > 1 )
		{
			auto hint_type = static_cast<FunctionSymbol*>(GlobalHelper::getTypeHint(this));
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of " + name);

			auto type_info = hint_type->getTypeInfo();

			if ( ov_func_type_info.symbols.find(type_info) == std::end(ov_func_type_info.symbols) )
			{
				auto sym = FunctionHelper::getViableOverload(ov_func, type_info.params_types);
				variable = new VariableSymbol(ov_func->getName(), sym);
			}
			else
				variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols[type_info]);
		}
		else
			variable = new VariableSymbol(ov_func->getName(), std::begin(ov_func_type_info.symbols)->second);

		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(variable->getType())->getScopedTypedName() + "]");
	}
	else
	{
		if ( variable->isField() )
		{
			auto _this = getScope()->resolve("this");

			auto sym = static_cast<VariableSymbol*>(_this);

			auto struc_scope = static_cast<StructSymbol*>(TypeHelper::removeReference(sym->getType()));

			CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(sym)) + "]");
			CodeGen::emit("lea rax, [rax - " + std::to_string(struc_scope->get_valloc()->getAddress(variable)) + "]");
		}
		else
		{
			auto hint = GlobalHelper::getTypeHint(this);
			if ( hint != nullptr )
				CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(hint)->getScopedTypedName() + "]");
			else
				CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(variable)) + "]");
		}
	}
}
	
vector<AST*> VariableNode::getChildren() const { return { }; }
