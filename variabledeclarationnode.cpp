#include "variabledeclarationnode.hpp"
#include "classvariablesymbol.hpp"

VariableDeclarationNode::VariableDeclarationNode(string name, TypeInfo type_info, bool is_field, const vector<ExprNode*>& constructor_call_params) : name(name),
																		     type_info(type_info),
																		     is_field(is_field),
																		     constructor_call_params(constructor_call_params)
{
    definedSymbol = new VariableSymbol(name, nullptr, (is_field ? VariableSymbolType::FIELD : VariableSymbolType::SIMPLE));
    resolved_constructor = nullptr;
}

VariableDeclarationNode::~VariableDeclarationNode() 
{ 
	delete definedSymbol; 

	for ( auto i : constructor_call_params )
		delete i;
}

Symbol* VariableDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void VariableDeclarationNode::build_scope()
{
    for ( auto i : constructor_call_params )
    {
		i->setScope(getScope());
		i->build_scope();
    }
}


void VariableDeclarationNode::check(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( !is_field )
	{
		string type_name = type_info.getTypeName();

		Type *_type = TypeHelper::fromTypeInfo(type_info, getScope(), template_sym, expr);

		if ( _type->getTypeKind() != TypeKind::STRUCT )
			throw SemanticError("No such struct " + type_name);

		StructSymbol *type = static_cast<StructSymbol*>(_type);
		resolved_constructor = CallHelper::callCheck(type_name, type, constructor_call_params, template_sym, expr);
	}
}

void VariableDeclarationNode::gen(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
	if ( !is_field )
	{   
		CodeGen::genCallCode(resolved_constructor, constructor_call_params, template_sym, expr, 
				[&]() {  CodeGen::emit("lea rax, [" + resolved_constructor->getScopedTypedName() + "]"); },
				[&]() { CodeGen::emit("lea rax, [rbp - " + std::to_string(getScope()->get_valloc()->getAddress(definedSymbol)) + "]"); }
		);
	}
}

void VariableDeclarationNode::define(const TemplateStructSymbol *template_sym, std::vector<ExprNode*> expr)
{
    if ( template_sym && template_sym->isIn(type_info.getTypeName()) )
    {
		auto replace = template_sym->getReplacement(type_info.getTypeName(), expr);

		auto sym = TypeHelper::removeReference(replace->getType());
		
		type_info = TypeInfo(static_cast<ClassVariableSymbol*>(sym)->sym->getName(), type_info.getIsRef(), type_info.getTemplateParams());
    }
    
    auto var_type = TypeHelper::fromTypeInfo(type_info, getScope(), template_sym, expr);
    
    if ( var_type == BuiltIns::void_type )
		throw SemanticError("can't declare a variable of 'void' type.");
    
    definedSymbol->setType(var_type);
    getScope()->accept(new VariableSymbolDefine(definedSymbol));
}

AST* VariableDeclarationNode::copyTree() const
{
    vector<ExprNode*> params(constructor_call_params.size());

    std::transform(std::begin(constructor_call_params), std::end(constructor_call_params), std::begin(params), [&] (ExprNode *expr) { return static_cast<ExprNode*>(expr->copyTree()); });
    
    return new VariableDeclarationNode(name, type_info, is_field, params);
}

vector<AST*> VariableDeclarationNode::getChildren() const { return vector<AST*>(std::begin(constructor_call_params), std::end(constructor_call_params)); }
