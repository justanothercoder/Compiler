#include "dotnode.hpp"

DotNode::DotNode(ExprNode *base, string member_name) : base(base), member_name(member_name), base_type(nullptr), member(nullptr) { }

DotNode::~DotNode() { delete base; }

void DotNode::check(const TemplateInfo& template_info)
{
	base->check(template_info);

	auto _base_type = base->getType();

	base_type = dynamic_cast<StructSymbol*>(_base_type.type);

	if ( base_type == nullptr )
		throw SemanticError("left side of '.' is not an instance of struct.");

	member = dynamic_cast<VariableSymbol*>(base_type->resolveMember(member_name));

	if ( member == nullptr )
		throw SemanticError(member_name + " is not member of " + base_type->getName());
}

VariableType DotNode::getType() const 
{ 
	auto type = member->getType();
	type.is_ref = true;

	return type; 
}

AST* DotNode::copyTree() const { return new DotNode(static_cast<ExprNode*>(base->copyTree()), member_name); }

void DotNode::gen(const TemplateInfo& template_info)
{    
	base->gen(template_info);
	
	auto member_type = member->getType();

	if ( member_type.is_ref )
		CodeGen::emit("mov rax, [rax - " + std::to_string(base_type->get_valloc()->getAddress(member)) + "]");
	else if ( member_type.type->getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
	{
		auto ov_func = static_cast<OverloadedFunctionSymbol*>(member_type.type);

		auto ov_func_type_info = ov_func->getTypeInfo();

		if ( ov_func_type_info.overloads.size() > 1 )
		{
			auto hint_type = GlobalHelper::getTypeHint(this);
			if ( hint_type == nullptr )
				throw SemanticError("multiple overloads of " + base_type->getName() + "::" + member->getName());

			member = new VariableSymbol(member_name, VariableType(ov_func_type_info.symbols[static_cast<FunctionSymbol*>(hint_type)->getTypeInfo()]));
		}
		else
			member = new VariableSymbol(ov_func->getName(), VariableType(std::begin(ov_func_type_info.symbols)->second));

//		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(member->getType().type)->getScopedTypedName() + "]");
	}
//	else if ( member_type.type->getTypeKind() == TypeKind::FUNCTION ) 
//		CodeGen::emit("lea rax, [" + static_cast<FunctionSymbol*>(member->getType().type)->getScopedTypedName() + "]");
	else
		CodeGen::emit("lea rax, [rax - " + std::to_string(base_type->get_valloc()->getAddress(member)) + "]");
}
	
vector<AST*> DotNode::getChildren() const { return {base}; }
