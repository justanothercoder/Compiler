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
    {
        auto ov_func = static_cast<OverloadedFunctionSymbol*>(BuiltIns::global_scope -> resolve("operator[]"));
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> base -> getType(), node -> expr -> getType()});

        auto base_value = ValueInfo{node -> base -> getType(), node -> base -> isLeftValue()};
        auto expr_value = ValueInfo{node -> expr -> getType(), node -> expr -> isLeftValue()};
            
        node -> call_info = ov_func -> resolveCall({base_value, expr_value});
    }
    else
    {
        assert(node -> base -> getType().unqualified() -> getTypeKind() == TypeKind::STRUCT); 
        auto base_type = static_cast<const StructSymbol*>(node -> base -> getType().unqualified());
        
        auto ov_func = static_cast<OverloadedFunctionSymbol*>(base_type -> resolve("operator[]"));
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> expr -> getType()});
        
        auto expr_value = ValueInfo{node -> expr -> getType(), node -> expr -> isLeftValue()};
        node -> call_info = ov_func -> resolveCall({expr_value});
    }
}

void CheckVisitor::visit(UnaryNode *node)
{
    node -> exp -> accept(*this);

    auto type = static_cast<const StructSymbol*>(node -> exp -> getType().unqualified());
    
    auto ov_func = static_cast<OverloadedFunctionSymbol*>(type -> resolve(node -> getOperatorName()));
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

    auto type = static_cast<const StructSymbol*>(fromTypeInfo(node -> type_info, node -> scope).base());

    for ( auto param : node -> params )
        param -> accept(*this);        

    std::vector<ValueInfo> arguments;
    for ( auto param : node -> params )
        arguments.emplace_back(param -> getType(), param -> isLeftValue());

    auto ov_func = static_cast<OverloadedFunctionSymbol*>(type -> resolve(type -> getName()));
    node -> call_info = ov_func -> resolveCall(arguments);
}

void CheckVisitor::visit(BinaryOperatorNode *node)
{
    visitChildren(node);
        
    auto lhs_type = node -> lhs -> getType();
    auto lhs_unqualified = lhs_type.unqualified();

    if ( lhs_unqualified -> getTypeKind() == TypeKind::STRUCT )
    {
        auto ov_func = static_cast<OverloadedFunctionSymbol*>(static_cast<const StructSymbol*>(lhs_unqualified) -> resolve(node -> getOperatorName()));
        node -> call_info = ov_func -> resolveCall({{node -> rhs -> getType(), node -> rhs -> isLeftValue()}});
    }
    else
    {
        auto ov_func = static_cast<OverloadedFunctionSymbol*>(node -> scope -> resolve(node -> getOperatorName()));
        node -> call_info = ov_func -> resolveCall({{node -> lhs -> getType(), node -> lhs -> isLeftValue()}, {node -> rhs -> getType(), node -> rhs -> isLeftValue()}});                
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

            assert(var_type.unqualified() -> getTypeKind() == TypeKind::STRUCT);
            auto struct_symbol = static_cast<const StructSymbol*>(var_type.unqualified());

            for ( auto param : node -> constructor_params )
                param -> accept(*this);
            
            std::vector<VariableType> params;
            std::vector<ValueInfo> arguments;

            for ( auto param : node -> constructor_params )
            {
                arguments.emplace_back(param -> getType(), param -> isLeftValue());
                params.push_back(param -> getType());
            }

            auto ov_func = static_cast<OverloadedFunctionSymbol*>(struct_symbol -> resolve(struct_symbol -> getName()));
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
        
        std::vector<VariableType> params;

        for ( auto param : node -> params )
            params.push_back(param -> getType());

        auto ov_func = static_cast<OverloadedFunctionSymbol*>(type -> resolve("operator()"));
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator()", params);

        try
        {
            std::vector<ValueInfo> arguments;

            for ( auto param : node -> params )
                arguments.emplace_back(param -> getType(), param -> isLeftValue());

            node -> call_info = ov_func -> resolveCall(arguments);
        }
        catch ( NoViableOverloadError& e ) { throw NoViableOverloadError("operator()", params); }
    }
    else
    {
        auto ov_func = static_cast<const OverloadedFunctionSymbol*>(caller_type.unqualified());
        
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
    {
        auto ov_func = static_cast<OverloadedFunctionSymbol*>(static_cast<const StructSymbol*>(unqualified_type) -> resolve(unqualified_type -> getName()));
        ov_func -> resolveCall({{node -> expr -> getType(), node -> expr -> isLeftValue()}});
    }
}

void CheckVisitor::visit(UnsafeBlockNode* node)
{
    node -> block -> accept(*this);
}

void CheckVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto type = node -> expr -> getType().unqualified();

    assert(type -> getTypeKind() == TypeKind::STRUCT);

    auto ov_func = static_cast<OverloadedFunctionSymbol*>(static_cast<const StructSymbol*>(type) -> resolve(type -> getName()));
    node -> call_info = ov_func -> resolveCall({{node -> expr -> getType(), node -> expr -> isLeftValue()}});
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
