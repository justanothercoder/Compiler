#include "newexpressionnode.hpp"

NewExpressionNode::NewExpressionNode(TypeInfo type_info, vector<ExprNode*> params) : type_info(type_info), params(params), resolved_constructor(nullptr) { }

NewExpressionNode::~NewExpressionNode()
{
	for ( auto i : params )
		delete i;
}

void NewExpressionNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	string name = type_info.getTypeName();

	StructSymbol *type = static_cast<StructSymbol*>(TypeHelper::fromTypeInfo(type_info, this->getScope()));

	Symbol *_constr = type->resolve(name);
	if ( _constr->getSymbolType() != SymbolType::VARIABLE )	
		throw SemanticError("No constructor");

	VariableSymbol *_constructor = static_cast<VariableSymbol*>(_constr);
	if ( _constructor->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
		throw SemanticError("No constructor");

	OverloadedFunctionSymbol *constructor = static_cast<OverloadedFunctionSymbol*>(_constructor->getType());

	vector<Type*> params_types;

	params_types.push_back(TypeHelper::getReferenceType(type));   

	for ( auto i : params ) 
	{
		i->check(template_sym, expr);
		params_types.push_back(i->getType());
	}

	resolved_constructor = FunctionHelper::getViableOverload(constructor, params_types);

	if ( resolved_constructor == nullptr )
		throw SemanticError("No viable overload of '" + name + "'.");

	auto resolved_constructor_type_info = resolved_constructor->getTypeInfo();

	for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
	{
		if ( resolved_constructor_type_info.getParamType(i)->isReference() && !params[i - 1]->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
	}
}

void NewExpressionNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	CodeGen::emit("push rdi");
	CodeGen::emit("lea rdi, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
	CodeGen::construct_object(getType(), resolved_constructor, params, 0, template_sym, expr);
	CodeGen::emit("pop rdi");
}

Type* NewExpressionNode::getType() const { return resolved_constructor->getTypeInfo().getReturnType(); }

AST* NewExpressionNode::copyTree() const 
{
	vector<ExprNode*> vec(params.size());
	std::transform(std::begin(params), std::end(params), std::begin(vec), [&](ExprNode *e) { return static_cast<ExprNode*>(e->copyTree()); });

	return new NewExpressionNode(type_info, vec);
}

vector<AST*> NewExpressionNode::getChildren() const { return vector<AST*>(std::begin(params), std::end(params)); }
