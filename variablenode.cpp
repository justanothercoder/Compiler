#include "variablenode.hpp"

VariableNode::VariableNode(string name) : name(name), variable(nullptr) { template_sym = nullptr; }

void VariableNode::build_scope() { }

void VariableNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym && template_sym->isIn(name) )
	{
		this->template_sym = const_cast<TemplateStructSymbol*>(template_sym);
		this->template_expr = expr;
		return;
	}

	Symbol *sym = this->getScope()->resolve(name);

	if ( sym == nullptr )
		throw SemanticError("No such symbol " + name);

	if ( sym->getSymbolType() == SymbolType::STRUCT )
	{
		variable = new VariableSymbol(name, new ClassVariableSymbol(static_cast<StructSymbol*>(sym)));
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

	return TypeHelper::getReferenceType(variable->getType());
}

bool VariableNode::isLeftValue() const { return true; }

bool VariableNode::isTemplateParam() const { return template_sym != nullptr; }

AST* VariableNode::copyTree() const
{
	return new VariableNode(name);
}

void VariableNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( template_sym && template_sym->isIn(name) )
	{
		auto replace = template_sym->getReplacement(name, expr);
		replace->gen(template_sym, expr);	
		return;
	}

	Type *var_type = variable->getType();

	if ( var_type->isReference() )
	{
		if ( variable->isField() )
		{
			VariableSymbol *sym = static_cast<VariableSymbol*>(this->getScope()->resolve("this"));

			Scope *struc_scope = static_cast<StructSymbol*>(sym->getType());

			CodeGen::emit("mov rax, [rbp - " + std::to_string(this->getScope()->getAddress(sym)) + "]");
			CodeGen::emit("mov rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
		}
		else
		{
			CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->getAddress(variable)) + "]");
		}
	}    
	else if ( var_type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		OverloadedFunctionSymbol *ov_func = static_cast<OverloadedFunctionSymbol*>(var_type);

		auto ov_func_type_info = ov_func->getTypeInfo();

		if ( ov_func_type_info.overloads.size() > 1 )
		{
			FunctionSymbol *hint_type = static_cast<FunctionSymbol*>(GlobalHelper::getTypeHint(this));
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of " + name);

			auto type_info = hint_type->getTypeInfo();

			if ( ov_func_type_info.symbols.find(type_info) == std::end(ov_func_type_info.symbols) )
			{
				auto sym = FunctionHelper::getViableOverload(ov_func, type_info.getParamsTypes());
				variable = new VariableSymbol(ov_func->getName(), sym);
			}
			else
				variable = new VariableSymbol(ov_func->getName(), ov_func_type_info.symbols[type_info]);
		}
		else
		{
			variable = new VariableSymbol(ov_func->getName(), std::begin(ov_func_type_info.symbols)->second);
		}

		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(variable->getType())->getScopedTypedName() + "]");
	}
	else
	{
		if ( variable->isField() )
		{
			Symbol *_this = this->getScope()->resolve("this");

			VariableSymbol *sym = static_cast<VariableSymbol*>(_this);

			Scope *struc_scope = static_cast<StructSymbol*>(static_cast<ReferenceType*>(sym->getType())->getReferredType());

			CodeGen::emit("mov rax, [rbp - " + std::to_string(getScope()->getAddress(sym)) + "]");
			CodeGen::emit("lea rax, [rax - " + std::to_string(struc_scope->getAddress(variable)) + "]");
		}
		else
		{
			CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->getAddress(variable)) + "]");
		}
	}
}
	
vector<AST*> VariableNode::getChildren() const { return { }; }
