#include "functiondeclarationnode.hpp"

FunctionDeclarationNode::FunctionDeclarationNode(string name, const vector< pair<string, TypeInfo> >& params, TypeInfo return_type_info, const vector<AST*>& statements, FunctionTraits traits) : name(name), params(params), return_type_info(return_type_info), statements(statements), traits(traits)
{
    definedSymbol = nullptr;
}

FunctionDeclarationNode::~FunctionDeclarationNode() { delete definedSymbol; }   

void FunctionDeclarationNode::build_scope()
{
    definedSymbol = new FunctionSymbol(name, FunctionTypeInfo(nullptr, { }), this->getScope(), traits);
    for ( auto i : statements )
    {
	i->setScope(definedSymbol);
	i->build_scope();
    }
}

Symbol* FunctionDeclarationNode::getDefinedSymbol() const { return definedSymbol; }

void FunctionDeclarationNode::define()
{
    Type *return_type = TypeHelper::fromTypeInfo(return_type_info, this->getScope());
    
    vector<Type*> params_types;
    
    if ( traits.is_method )
    {
	Type *_this_type = TypeHelper::getReferenceType(static_cast<StructSymbol*>(this->getScope()));
	params_types.push_back(_this_type);

	definedSymbol->define(new VariableSymbol("this", _this_type, VariableSymbolType::PARAM));
    }
    
    for ( auto i : params )
    {
	Type *param_type = TypeHelper::fromTypeInfo(i.second, this->getScope());	
	params_types.push_back(param_type);

	definedSymbol->define(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM));
    }

    FunctionTypeInfo function_type_info(return_type, params_types);

    definedSymbol->setTypeInfo(function_type_info);

    this->getScope()->define(definedSymbol);
    
    for ( auto i : statements )
	i->define();	
}

void FunctionDeclarationNode::check()
{
    definedSymbol->recalc_scope_address();
    
    for ( auto i : statements )
	i->check();
}

void FunctionDeclarationNode::gen()
{
    string scoped_typed_name = definedSymbol->getScopedTypedName();
    
    CodeGen::emit("jmp _~" + scoped_typed_name);
    CodeGen::emit(scoped_typed_name + ":");
    CodeGen::emit("push rbp");
    CodeGen::emit("mov rbp, rsp");

    for ( auto i : statements )
	i->gen();
    
    CodeGen::emit("mov rsp, rbp");
    CodeGen::emit("pop rbp");
    CodeGen::emit("ret");
    CodeGen::emit("_~" + scoped_typed_name + ":");
}

void FunctionDeclarationNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto t : statements )
	t->template_check(template_sym, expr);
}

bool FunctionDeclarationNode::isTemplated() const
{
    return std::accumulate(std::begin(statements), std::end(statements), false, [](bool a, bool b) { return a || b; });
}

void FunctionDeclarationNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    if ( return_type_info.getTypeName() == template_sym->getName() )
    {
	return_type_info = TypeInfo(
	    static_cast<StructSymbol*>(this->getScope())->getName(),
	    return_type_info.getIsRef(),
	    return_type_info.getTemplateParams()
	    );
    }
    
    Type *return_type = TypeHelper::fromTypeInfo(return_type_info, this->getScope());

    vector<Type*> params_types;
    
    if ( traits.is_method )
    {
	Type *_this_type = TypeHelper::getReferenceType(static_cast<StructSymbol*>(this->getScope()));
	params_types.push_back(_this_type);

	definedSymbol->define(new VariableSymbol("this", _this_type, VariableSymbolType::PARAM));
    }
    
    for ( auto i : params )
    {
	Type *param_type = TypeHelper::fromTypeInfo(i.second, this->getScope());	
	params_types.push_back(param_type);

	definedSymbol->define(new VariableSymbol(i.first, param_type, VariableSymbolType::PARAM));
    }

    FunctionTypeInfo function_type_info(return_type, params_types);

    definedSymbol->setTypeInfo(function_type_info);

    this->getScope()->define(definedSymbol);

    for ( auto t : statements )
	t->template_define(template_sym, expr);
}

AST* FunctionDeclarationNode::copyTree() const
{
    vector<AST*> stats;
    std::transform(std::begin(statements), std::end(statements), std::back_inserter(stats), [&](AST *t) { return t->copyTree(); });
	
    return new FunctionDeclarationNode(name, params, return_type_info, stats, traits);
}

void FunctionDeclarationNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    
}
