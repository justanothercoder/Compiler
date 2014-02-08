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

VariableDeclarationNode::~VariableDeclarationNode() { delete definedSymbol; }

Symbol* VariableDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void VariableDeclarationNode::build_scope()
{
    for ( auto i : constructor_call_params )
    {
	i->setScope(this->getScope());
	i->build_scope();
    }
}

void VariableDeclarationNode::define()
{
    Type *var_type = TypeHelper::fromTypeInfo(type_info, this->getScope());

    if ( var_type == BuiltIns::void_type )
		throw SemanticError("can't declare a variable of 'void' type");
    
    definedSymbol->setType(var_type);
    this->getScope()->define(definedSymbol);
}

void VariableDeclarationNode::check()
{
    if ( !is_field )
    {
		string type_name = type_info.getTypeName();
		
		Symbol *_type = this->getScope()->resolve(type_name);

		if ( _type->getSymbolType() != SymbolType::STRUCT )
			throw SemanticError("No such struct " + type_name);
		
		StructSymbol *type = static_cast<StructSymbol*>(_type);

		Symbol *_constr = type->resolve(type_name);
		if ( _constr->getSymbolType() != SymbolType::VARIABLE )	
			throw SemanticError("No constructor");

		VariableSymbol *_constructor = static_cast<VariableSymbol*>(_constr);
		if ( _constructor->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
			throw SemanticError("No constructor");

		OverloadedFunctionSymbol *constructor = static_cast<OverloadedFunctionSymbol*>(_constructor->getType());

		vector<Type*> params_types;
		params_types.push_back(TypeHelper::getReferenceType(definedSymbol->getType()));   

		for ( auto t : constructor_call_params )
			t->check();
		std::transform(std::begin(constructor_call_params), std::end(constructor_call_params), std::back_inserter(params_types), [](ExprNode *t) { return t->getType(); });
		
		resolved_constructor = FunctionHelper::getViableOverload(constructor, params_types);

		if ( resolved_constructor == nullptr )
			throw SemanticError("No viable overload of '" + type_name + "'.");
		
		auto resolved_constructor_type_info = resolved_constructor->getTypeInfo();

		for ( int i = resolved_constructor_type_info.getNumberOfParams() - 1; i >= 1; --i )
		{
			if ( resolved_constructor_type_info.getParamType(i)->isReference() && !constructor_call_params[i - 1]->isLeftValue() )
			throw SemanticError("parameter is not an lvalue.");
		}
    }
}

void VariableDeclarationNode::gen()
{
    if ( !is_field )
    {      
	CodeGen::emit("lea rdi, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
	CodeGen::emit("sub rsp, " + std::to_string(definedSymbol->getType()->getSize()));

	CodeGen::emit("push rsi");
	CodeGen::emit("lea rsi, [" + resolved_constructor->getScopedTypedName() + "]");

	CodeGen::genCallCode(resolved_constructor, constructor_call_params);
	CodeGen::emit("pop rsi");
    }
}

void VariableDeclarationNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    if ( template_sym->isIn(type_info.getTypeName()) )
    {
	auto replace = template_sym->getReplacement(type_info.getTypeName(), expr);

	auto sym = static_cast<ReferenceType*>(replace->getType())->getReferredType();
	
	type_info = TypeInfo(static_cast<ClassVariableSymbol*>(sym)->sym->getName(), type_info.getIsRef(), type_info.getTemplateParams());
    }
    
    Type *var_type = TypeHelper::fromTypeInfo(type_info, this->getScope(), template_sym, expr);
    
    if ( var_type == BuiltIns::void_type )
	throw SemanticError("can't declare a variable of 'void' type.");
    else if ( template_sym->isIn(dynamic_cast<Symbol*>(var_type)) )
    {
	auto replace = template_sym->getReplacement(dynamic_cast<Symbol*>(var_type), expr);
	replace->setScope(this->getScope());
	replace->template_check(template_sym, expr);
	var_type = static_cast<ClassVariableSymbol*>(static_cast<ReferenceType*>(replace->getType())->getReferredType())->sym;
    }
    
    definedSymbol->setType(var_type);
    this->getScope()->define(definedSymbol);
}

void VariableDeclarationNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    if ( !is_field )
    {
	
    }
}

AST* VariableDeclarationNode::copyTree() const
{
    vector<ExprNode*> params;

    std::transform(std::begin(constructor_call_params), std::end(constructor_call_params), std::back_inserter(params), [&] (ExprNode *expr) { return static_cast<ExprNode*>(expr->copyTree()); });
    
    return new VariableDeclarationNode(name, type_info, is_field, params);
}

void VariableDeclarationNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
