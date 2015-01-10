#include "checkvisitor.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "structsymbol.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "functionsymbol.hpp"
#include "variabledeclarationnode.hpp"
#include "addrnode.hpp"
#include "dotnode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "callnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "nullnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "templatefunctionnode.hpp"
#include "importnode.hpp"
#include "modulenode.hpp"
#include "typenode.hpp"
#include "functionnode.hpp"
#include "modulememberaccessnode.hpp"
#include "modulesymbol.hpp"
#include "compilableunit.hpp"
#include "comp.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "noviableoverloaderror.hpp"
#include "partiallyinstantiatedfunctionsymbol.hpp"
#include "templatefunctionsymbol.hpp"
#include "objecttype.hpp"
#include "logger.hpp"
    
OverloadedFunctionSymbol* CheckVisitor::resolveFunction(const Scope* scope, std::string name)
{
    auto func = scope -> resolve(name);
    assert(func -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);
    return static_cast<OverloadedFunctionSymbol*>(func);
}

void CheckVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}
    
ValueInfo CheckVisitor::valueOf(ExprNode* expr) { return {expr -> getType(), expr -> isLeftValue()}; }

void CheckVisitor::visit(BracketNode *node)
{
    visitChildren(node);

    if ( node -> base -> getType().base() -> getTypeKind() == TypeKind::ARRAY )
    {
        auto ov_func = resolveFunction(BuiltIns::global_scope, "operator[]");
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> base -> getType(), node -> expr -> getType()});
            
        node -> call_info = ov_func -> resolveCall({valueOf(node -> base), valueOf(node -> expr)});
    }
    else
    {
        assert(node -> base -> getType().unqualified() -> isObjectType());
        auto base_type = static_cast<const ObjectType*>(node -> base -> getType().unqualified());
        
        auto ov_func = base_type -> resolveMethod("operator[]");
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> expr -> getType()});
        
        node -> call_info = ov_func -> resolveCall({valueOf(node -> expr)});
    }
}

void CheckVisitor::visit(UnaryNode *node)
{
    node -> exp -> accept(*this);
    assert(node -> exp -> getType().unqualified() -> isObjectType());

    auto type = static_cast<const ObjectType*>(node -> exp -> getType().unqualified());
    
    auto ov_func = type -> resolveMethod(node -> getOperatorName());
    if ( ov_func == nullptr )
        throw NoViableOverloadError(node -> getOperatorName(), { });

    node -> call_info = ov_func -> resolveCall({ });
}

void CheckVisitor::visit(NewExpressionNode *node)
{
    for ( auto param : node -> type_info.template_params )
    {
        if ( param.which() == 0 )
            boost::get<ExprNode*>(param) -> accept(*this);
    }
    
    auto tp = fromTypeInfo(node -> type_info, node -> scope).base();
    assert(tp -> isObjectType());
    auto type = static_cast<const ObjectType*>(tp);

    for ( auto param : node -> params )
        param -> accept(*this);

    std::vector<ValueInfo> arguments;
    for ( auto param : node -> params )
        arguments.emplace_back(param -> getType(), param -> isLeftValue());

    auto ov_func = type -> resolveMethod(type -> getName());
    node -> call_info = ov_func -> resolveCall(arguments);
}

void CheckVisitor::visit(BinaryOperatorNode *node)
{
    visitChildren(node);
        
    auto lhs_type = node -> lhs -> getType();
    auto lhs_unqualified = lhs_type.unqualified();

    if ( lhs_unqualified -> isObjectType() )
    {
        auto ov_func = static_cast<const ObjectType*>(lhs_unqualified) -> resolveMethod(node -> getOperatorName());
        node -> call_info = ov_func -> resolveCall({valueOf(node -> rhs)});
    }
    else
    {
        auto ov_func = resolveFunction(node -> scope, node -> getOperatorName());
        node -> call_info = ov_func -> resolveCall({valueOf(node -> lhs), valueOf(node -> rhs)});                
    }        
}

void CheckVisitor::visit(StructDeclarationNode *node)
{
    /*
    	if ( node -> definedSymbol -> getDefaultConstructor() == nullptr )
    	{
    		auto default_constr = FunctionHelper::makeDefaultConstructor(node -> definedSymbol);
    		node -> definedSymbol -> define(default_constr);
    	}

    	if ( node -> definedSymbol -> getCopyConstructor() == nullptr )
    	{
    		auto copy_constr = FunctionHelper::makeDefaultCopyConstructor(node -> definedSymbol);
    		node -> definedSymbol -> define(copy_constr);
    	}
    */

    visitChildren(node);
}

void CheckVisitor::visit(VariableDeclarationNode *node)
{
    for ( auto param : node -> type_info.template_params )
    {
        if ( param.which() == 0 )
            boost::get<ExprNode*>(param) -> accept(*this);
    }

    if ( !node -> is_field )
    {
        if ( node -> type_info.is_ref || node -> type_info.pointer_depth > 0 )
        {
            for ( auto param : node -> constructor_params )
                param -> accept(*this);
        }
        else
        {
            auto var_type = fromTypeInfo(node -> type_info, node -> scope);

            assert(var_type.unqualified() -> isObjectType());
            auto struct_symbol = static_cast<const ObjectType*>(var_type.unqualified());

            for ( auto param : node -> constructor_params )
                param -> accept(*this);
            
            auto params    = std::vector<VariableType>{ };
            auto arguments = std::vector<ValueInfo>{ };

            for ( auto param : node -> constructor_params )
            {
                arguments.emplace_back(param -> getType(), param -> isLeftValue());
                params.push_back(param -> getType());
            }

            auto ov_func = struct_symbol -> resolveMethod(struct_symbol -> getName());
            if ( ov_func == nullptr )
                throw NoViableOverloadError(struct_symbol -> getName(), params);

            try
            {
                node -> call_info = ov_func -> resolveCall(arguments);
            }
            catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(struct_symbol -> getName(), params); }
        }
    }
}

void CheckVisitor::visit(AddrNode *node)
{
    node -> expr -> accept(*this);

    if ( node -> op == AddrOp::REF )
    {
        if ( !node -> expr -> isLeftValue() )
            throw SemanticError("expression is not an lvalue");
    }
    else
    {
        auto type = node -> expr -> getType().unqualified();

        if ( type -> getTypeKind() != TypeKind::POINTER )
            throw SemanticError("expression is not a pointer");
    }
}

void CheckVisitor::visit(DotNode *node)
{
    node -> base -> accept(*this);

    auto _base_type = node -> base -> getType();

    assert(_base_type.unqualified() -> isObjectType());
    node -> base_type = static_cast<const ObjectType*>(_base_type.unqualified());

    if ( node -> base_type == nullptr )
        throw SemanticError("'" + node -> base -> toString() + "' is not an instance of struct.");

    node -> member = node -> base_type -> resolveMember(node -> member_name);

    if ( node -> member == nullptr )
        throw SemanticError(node -> member_name + " is not member of " + node -> base_type -> getName());
}

void CheckVisitor::visit(ModuleMemberAccessNode* node)
{
    auto module_sym = Comp::getUnit(node -> name) -> module_symbol;
    assert(module_sym && module_sym -> getSymbolType() == SymbolType::MODULE);

    node -> member_sym = static_cast<ModuleSymbol*>(module_sym) -> resolve(node -> member);
}

void CheckVisitor::visit(ModuleNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name);
    assert(sym && sym -> getSymbolType() == SymbolType::MODULE);

    node -> module = static_cast<ModuleSymbol*>(sym);
}

void CheckVisitor::visit(TypeNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name);
    assert(sym && sym -> getSymbolType() == SymbolType::STRUCT);

    node -> type_symbol = static_cast<StructSymbol*>(sym);
}

void CheckVisitor::visit(FunctionNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name);
    assert(sym && sym -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);    

    node -> function = static_cast<OverloadedFunctionSymbol*>(sym);
}

void CheckVisitor::visit(TemplateFunctionNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name);
    assert(sym && sym -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);    

    auto ov_func = static_cast<OverloadedFunctionSymbol*>(sym);

    if ( node -> template_params.empty() )
        node -> function = ov_func;
    else
    {
        std::vector<TemplateParam> template_arguments;
        for ( auto param : node -> template_params )
        {
            if ( param.which() == 0 )
                template_arguments.emplace_back(*boost::get<ExprNode*>(param) -> getCompileTimeValue());
            else
                template_arguments.emplace_back(boost::get<TypeInfo>(param));
        }

        node -> function = new PartiallyInstantiatedFunctionSymbol(ov_func, template_arguments);
    }
}

void CheckVisitor::visit(VariableNode *node)
{
    const auto& template_info = node -> scope -> templateInfo();

    if ( template_info.sym && template_info.isIn(node -> name) )
    {
        auto replace = template_info.getReplacement(node -> name);

        if ( replace -> which() == 0 )
            throw SemanticError(node -> name + " is typename");

        node -> template_num = new NumberNode(std::to_string(boost::get<int>(*replace)));
        node -> template_num -> scope = node -> scope;
        node -> template_num -> build_scope();

        node -> template_num -> accept(*this);
        return;
    }
    
    auto sym = node -> scope -> resolve(node -> name);
    node -> variable = static_cast<VariableSymbol*>(sym);
}

void CheckVisitor::visit(CallNode *node)
{
    visitChildren(node);

    auto caller_type = node -> caller -> getType();

    if ( caller_type.unqualified() -> getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
    {
        if ( caller_type.unqualified() -> getTypeKind() != TypeKind::STRUCT )
            throw SemanticError("caller '" + node -> caller -> toString() + "' is not a function.");

        auto type = static_cast<const ObjectType*>(caller_type.unqualified());
        
        auto params    = std::vector<VariableType>{ };
        auto arguments = std::vector<ValueInfo>{ };

        for ( auto param : node -> params )
        {
            params.push_back(param -> getType());
            arguments.emplace_back(param -> getType(), param -> isLeftValue());
        }

        auto ov_func = type -> resolveMethod("operator()");
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator()", params);

        try
        {
            node -> call_info = ov_func -> resolveCall(arguments);
        }
        catch ( NoViableOverloadError& e ) { throw NoViableOverloadError("operator()", params); }
    }
    else
    {
        auto ov_func = static_cast<const FunctionalType*>(caller_type.unqualified());
        
        auto params    = std::vector<VariableType>{ };
        auto arguments = std::vector<ValueInfo>{ };
        
        for ( auto param : node -> params )
        {
            params.push_back(param -> getType());
            arguments.emplace_back(param -> getType(), param -> isLeftValue());
        }

        try
        {
            node -> call_info = ov_func -> resolveCall(arguments);
        }
        catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(ov_func -> getName(), params); }
    }
}

void CheckVisitor::visit(ReturnNode *node)
{
    node -> expr -> accept(*this);

    if ( node -> is_in_inline_call )
        return;

    if ( function_scopes.empty() )
        throw SemanticError("return is not in a function");

    node -> enclosing_func = function_scopes.top() -> func;

    auto unqualified_type = node -> expr -> getType().unqualified();
    if ( unqualified_type -> getTypeKind() != TypeKind::POINTER )
    {
        auto ov_func = static_cast<const ObjectType*>(unqualified_type) -> resolveMethod(unqualified_type -> getName());
        ov_func -> resolveCall({valueOf(node -> expr)});
    }
}

void CheckVisitor::visit(UnsafeBlockNode* node)
{
    node -> block -> accept(*this);
}

void CheckVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto type = node -> expr -> getType().unqualified();
    assert(type -> isObjectType());

    auto ov_func = static_cast<const ObjectType*>(type) -> resolveMethod(type -> getName());
    node -> call_info = ov_func -> resolveCall({valueOf(node -> expr)});
}

void CheckVisitor::visit(TemplateStructDeclarationNode* node)
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}
    
void CheckVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}

void CheckVisitor::visit(FunctionDeclarationNode *node) 
{ 
    function_scopes.push(node -> func_scope);
    visitChildren(node); 
    function_scopes.pop();
}

void CheckVisitor::visit(IfNode *node)                  { visitChildren(node); }
void CheckVisitor::visit(ForNode *node)                 { visitChildren(node); }
void CheckVisitor::visit(WhileNode *node)               { visitChildren(node); }
void CheckVisitor::visit(StatementNode *node)           { visitChildren(node); }

void CheckVisitor::visit(NullNode*) { } 
void CheckVisitor::visit(BreakNode* ) { }
void CheckVisitor::visit(NumberNode *) { }
void CheckVisitor::visit(StringNode *) { }
void CheckVisitor::visit(ExternNode *) { }
void CheckVisitor::visit(ImportNode* ) { }
