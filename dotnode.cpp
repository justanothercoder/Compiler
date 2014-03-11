#include "dotnode.hpp"

DotNode::DotNode(ExprNode *base, string member_name) : base(base), member_name(member_name), base_type(nullptr), member(nullptr) { }

DotNode::~DotNode() { delete base; }

void DotNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	base->check(template_sym, expr);

	Type *_base_type = TypeHelper::removeReference(base->getType());

	base_type = dynamic_cast<StructSymbol*>(_base_type);

	if ( base_type == nullptr )
		throw SemanticError("left side of '.' is not a struct instance.");

	member = dynamic_cast<VariableSymbol*>(base_type->resolveMember(member_name));

	if ( member == nullptr )
		throw SemanticError(member_name + " is not member of " + base_type->getName());
}

Type* DotNode::getType() const { return TypeHelper::addReference(member->getType()); }

void DotNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr) { }

AST* DotNode::copyTree() const { return new DotNode(static_cast<ExprNode*>(base->copyTree()), member_name); }

void DotNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{    
	base->gen(template_sym, expr);

	auto member_type = member->getType();

	if ( member_type->isReference() )
		CodeGen::emit("mov rax, [rax - " + std::to_string(base_type->get_valloc()->getAddress(member)) + "]");
	else if ( member_type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(member_type);

		auto ov_func_type_info = ov_func->getTypeInfo();

		if ( ov_func_type_info.overloads.size() > 1 )
		{
			Type *hint_type = GlobalHelper::getTypeHint(this);
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of " + base_type->getName() + "::" + member->getName());

			member = new VariableSymbol(member_name, ov_func_type_info.symbols[static_cast<FunctionSymbol*>(hint_type)->getTypeInfo()]);
		}
		else
			member = new VariableSymbol(ov_func->getName(), std::begin(ov_func_type_info.symbols)->second);

		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(member->getType())->getScopedTypedName() + "]");
	}
	else if ( member_type->getTypeKind() == TypeKind::FUNCTION ) 
	{
		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(member->getType())->getScopedTypedName() + "]");
	}
	else
		CodeGen::emit("lea rax, [rax - " + std::to_string(base_type->get_valloc()->getAddress(member)) + "]");
}
	
vector<AST*> DotNode::getChildren() const { return {base}; }
