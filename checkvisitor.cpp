#include "checkvisitor.hpp"
#include "callhelper.hpp"
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
#include "builtins.hpp"
#include "compilableunit.hpp"
#include "comp.hpp"

#include "noviableoverloaderror.hpp"

#include "logger.hpp"

void CheckVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void CheckVisitor::visit(BracketNode *node)
{
    visitChildren(node);

    if ( node -> base -> getType().base() -> getTypeKind() == TypeKind::ARRAY )
        node -> call_info = CallHelper::callCheck("operator[]", BuiltIns::global_scope, {node -> base, node -> expr});
    else
    {
        assert(node -> base -> getType().unqualified() -> getTypeKind() == TypeKind::STRUCT); 
        auto base_type = static_cast<const StructSymbol*>(node -> base -> getType().unqualified());
        node -> call_info = CallHelper::callCheck("operator[]", base_type, {node -> expr});
    }
}

void CheckVisitor::visit(UnaryNode *node)
{
    node -> exp -> accept(*this);

    auto type = static_cast<const StructSymbol*>(node -> exp -> getType().unqualified());
    node -> call_info = CallHelper::callCheck(node -> getOperatorName(), type, { });
}

void CheckVisitor::visit(NewExpressionNode *node)
{
    for ( auto param : node -> type_info.template_params )
    {
        if ( param.which() == 0 )
            boost::get<ExprNode*>(param) -> accept(*this);
    }

    auto type = static_cast<const StructSymbol*>(fromTypeInfo(node -> type_info, node -> scope).base());

    for ( auto param : node -> params )
        param -> accept(*this);        

    node -> call_info = CallHelper::callCheck(type -> getName(), type, node -> params);
}

void CheckVisitor::visit(BinaryOperatorNode *node)
{
    visitChildren(node);
    try
    {
        auto lhs_type = node -> lhs -> getType();
        auto lhs_unqualified = lhs_type.unqualified();

        if ( lhs_unqualified -> getTypeKind() == TypeKind::STRUCT )
            node -> call_info = CallHelper::callCheck(node -> getOperatorName(), static_cast<const StructSymbol*>(lhs_unqualified), {node -> rhs});
        else
            throw SemanticError("");
    }
    catch ( SemanticError& e )
    {
        node -> call_info = CallHelper::callCheck(node -> getOperatorName(), node -> scope, {node -> lhs, node -> rhs});
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

            auto struct_symbol = static_cast<const StructSymbol*>(var_type.unqualified());

            for ( auto param : node -> constructor_params )
                param -> accept(*this);

            node -> call_info = CallHelper::callCheck(struct_symbol -> getName(), struct_symbol, node -> constructor_params);
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

    assert(_base_type.unqualified() -> getTypeKind() == TypeKind::STRUCT);
    node -> base_type = static_cast<const StructSymbol*>(_base_type.unqualified());

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

    node -> function = static_cast<OverloadedFunctionSymbol*>(sym);
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

    if ( sym == nullptr )
        throw SemanticError("No such symbol '" + node -> name + "'");

    if ( sym -> getSymbolType() != SymbolType::VARIABLE )
        throw SemanticError("'" + node -> name + "' is not a variable.");

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

        auto type = static_cast<const StructSymbol*>(caller_type.unqualified());
        node -> call_info = CallHelper::callCheck("operator()", type, node -> params);
    }
    else
    {
        auto ov_func = static_cast<const OverloadedFunctionSymbol*>(caller_type.unqualified());
/*
        std::vector<VariableType> params;
        
        if ( ov_func -> isMethod() )
            params.push_back(ov_func -> getBaseType());

        for ( auto param : node -> params )
            params.push_back(param -> getType());

        Logger::log("Checking '" + node -> toString() + "'");

        auto func = ov_func -> getViableOverload(FunctionTypeInfo(params));

        if ( func == nullptr )
            throw SemanticError("No viable overload for " + ov_func -> getName() + " with params " + FunctionTypeInfo(params).toString());

        node -> call_info = CallHelper::getCallInfo(func, node -> params);
*/
        std::vector<VariableType> params;
        
        for ( auto param : node -> params )
            params.push_back(param -> getType());

        Logger::log("Checking '" + node -> toString() + "'");
        
        try
        {
            std::vector<ValueInfo> arguments;
            
            for ( auto param : node -> params )
                arguments.emplace_back(param -> getType(), param -> isLeftValue());

            node -> call_info = ov_func -> resolveCall(arguments);
        }
        catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(ov_func -> getName(), params); }
    }
}

void CheckVisitor::visit(ReturnNode *node)
{
    if ( node -> is_in_inline_call )
    {
        node -> expr -> accept(*this);
        return;
    }

    auto scope = node -> scope;
    while ( scope != nullptr && dynamic_cast<FunctionScope*>(scope) == nullptr )
        scope = scope -> enclosingScope();

    if ( scope == nullptr )
        throw SemanticError("return is not in a function");

    node -> enclosing_func = static_cast<FunctionScope*>(scope) -> func;

    node -> expr -> accept(*this);
   
    auto unqualified_type = node -> expr -> getType().unqualified();
    if ( unqualified_type -> getTypeKind() != TypeKind::POINTER )
        CallHelper::callCheck(unqualified_type -> getName(), static_cast<const StructSymbol*>(unqualified_type), {node -> expr});
}

void CheckVisitor::visit(UnsafeBlockNode* node)
{
    node -> block -> accept(*this);
}

void CheckVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto type = node -> expr -> getType().unqualified();

    if ( type -> getTypeKind() == TypeKind::STRUCT )
        node -> call_info = CallHelper::callCheck(type -> getName(), static_cast<const StructSymbol*>(type), {node -> expr});
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

void CheckVisitor::visit(IfNode *node)                  { visitChildren(node); }
void CheckVisitor::visit(ForNode *node)                 { visitChildren(node); }
void CheckVisitor::visit(WhileNode *node)               { visitChildren(node); }
void CheckVisitor::visit(FunctionDeclarationNode *node) { visitChildren(node); }
void CheckVisitor::visit(StatementNode *node)           { visitChildren(node); }

void CheckVisitor::visit(NullNode*) { } 
void CheckVisitor::visit(BreakNode* ) { }
void CheckVisitor::visit(NumberNode *) { }
void CheckVisitor::visit(StringNode *) { }
void CheckVisitor::visit(ExternNode *) { }
void CheckVisitor::visit(ImportNode* ) { }
