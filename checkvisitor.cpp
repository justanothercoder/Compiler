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
    
const OverloadedFunctionSymbol* CheckVisitor::resolveFunction(const Scope* scope, const std::string& name)
{
    auto func = scope -> resolve(name);
    assert(func -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);
    return static_cast<const OverloadedFunctionSymbol*>(func);
}

void CheckVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

ValueInfo CheckVisitor::valueOf(const ExprNode* expr) { return {expr -> getType(), expr -> isLeftValue()}; }

std::vector<ValueInfo> CheckVisitor::extractArguments(const std::vector< std::unique_ptr<ExprNode> >& params)
{
    auto result = std::vector<ValueInfo>{ };

    for ( const auto& param : params )
        result.emplace_back(param -> getType(), param -> isLeftValue());

    return result;
}

void CheckVisitor::visit(BracketNode *node)
{
    visitChildren(node);

    if ( node -> base() -> getType().unqualified() -> getTypeKind() == TypeKind::ARRAY )
    {
        auto ov_func = resolveFunction(BuiltIns::global_scope.get(), "operator[]");
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> base() -> getType(), node -> expr() -> getType()});
            
        node -> callInfo(ov_func -> resolveCall({valueOf(node -> base()), valueOf(node -> expr())}));
    }
    else
    {
        assert(node -> base() -> getType().unqualified() -> isObjectType());
        auto base_type = static_cast<const ObjectType*>(node -> base() -> getType().unqualified());
        
        auto ov_func = base_type -> resolveMethod("operator[]");
        if ( ov_func == nullptr )
            throw NoViableOverloadError("operator[]", {node -> expr() -> getType()});
        
        node -> callInfo(ov_func -> resolveCall({valueOf(node -> expr())}));
    }
}

void CheckVisitor::visit(UnaryNode* node)
{
    node -> expr() -> accept(*this);
    assert(node -> expr() -> getType().unqualified() -> isObjectType());

    auto type = static_cast<const ObjectType*>(node -> expr() -> getType().unqualified());
    
    auto ov_func = type -> resolveMethod(node -> getOperatorName());
    if ( ov_func == nullptr )
        throw NoViableOverloadError(node -> getOperatorName(), std::vector<ValueInfo>{ });

    node -> callInfo(ov_func -> resolveCall({ }));
}

void CheckVisitor::visit(NewExpressionNode* node)
{
    for ( auto param : node -> typeInfo().templateParams() )
    {
        if ( param.which() == 0 )
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
    }
    
    auto tp = fromTypeInfo(node -> typeInfo(), node -> scope.get()).base();
    assert(tp -> isObjectType());
    auto type = static_cast<const ObjectType*>(tp);

    for ( const auto& param : node -> params() )
        param -> accept(*this);

    auto arguments = extractArguments(node -> params());

    auto ov_func = type -> resolveMethod(type -> getName());
    node -> callInfo(ov_func -> resolveCall(arguments));
}

void CheckVisitor::visit(BinaryOperatorNode* node)
{
    visitChildren(node);
        
    auto lhs_type = node -> lhs() -> getType();
    auto lhs_unqualified = lhs_type.unqualified();

    if ( lhs_unqualified -> isObjectType() )
    {
        auto ov_func = static_cast<const ObjectType*>(lhs_unqualified) -> resolveMethod(node -> getOperatorName());
        node -> callInfo(ov_func -> resolveCall({valueOf(node -> rhs())}));
    }
    else
    {
        auto ov_func = resolveFunction(node -> scope.get(), node -> getOperatorName());
        node -> callInfo(ov_func -> resolveCall({valueOf(node -> lhs()), valueOf(node -> rhs())}));
    }

//    node -> checkCall();
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

void CheckVisitor::visit(VariableDeclarationNode* node)
{
    for ( auto param : node -> typeInfo().templateParams() )
    {
        if ( param.which() == 0 )
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
    }

    if ( !node -> isField() )
    {
        if ( node -> typeInfo().isRef() || !node -> typeInfo().modifiers().empty() )
        {
            for ( const auto& param : node -> constructorParams() )
                param -> accept(*this);
        }
        else
        {
            auto var_type = fromTypeInfo(node -> typeInfo(), node -> scope.get());
            assert(var_type.unqualified() -> isObjectType());

            auto struct_symbol = static_cast<const ObjectType*>(var_type.unqualified());

            for ( const auto& param : node -> constructorParams() )
                param -> accept(*this);
            
            auto arguments = extractArguments(node -> constructorParams());

            auto ov_func = struct_symbol -> resolveMethod(struct_symbol -> getName());
            if ( ov_func == nullptr )
                throw NoViableOverloadError(struct_symbol -> getName(), arguments);

            try {
                node -> callInfo(ov_func -> resolveCall(arguments));
            }
            catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(struct_symbol -> getName(), arguments); }
        }
    }
}

void CheckVisitor::visit(AddrNode* node)
{
    node -> expr() -> accept(*this);

    if ( node -> op() == AddrOp::REF )
    {
        if ( !node -> expr() -> isLeftValue() )
            throw SemanticError("expression is not an lvalue");
    }
    else
    {
        auto type = node -> expr() -> getType().unqualified();

        if ( type -> getTypeKind() != TypeKind::POINTER )
            throw SemanticError("expression is not a pointer");
    }
}

void CheckVisitor::visit(DotNode* node)
{
    node -> base() -> accept(*this);

    auto _base_type = node -> base() -> getType();
    assert(_base_type.unqualified() -> isObjectType());

    node -> baseType(static_cast<const ObjectType*>(_base_type.unqualified()));

    if ( node -> baseType() == nullptr )
        throw SemanticError("'" + node -> base() -> toString() + "' is not an instance of struct.");

    node -> member(node -> baseType() -> resolveMember(node -> memberName()));

    if ( node -> member() == nullptr )
        throw SemanticError(node -> memberName() + " is not member of " + node -> baseType() -> getName());
}

void CheckVisitor::visit(ModuleMemberAccessNode* node)
{
    auto module_sym = Comp::getUnit(node -> name()) -> module_symbol;
    assert(module_sym && module_sym -> getSymbolType() == SymbolType::MODULE);

    node -> memberSymbol(static_cast<const ModuleSymbol*>(module_sym.get()) -> resolve(node -> member()));
}

void CheckVisitor::visit(ModuleNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> getSymbolType() == SymbolType::MODULE);

    node -> module(static_cast<const ModuleSymbol*>(sym));
}

void CheckVisitor::visit(TypeNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> getSymbolType() == SymbolType::STRUCT);

    node -> typeSymbol(static_cast<const StructSymbol*>(sym));
}

void CheckVisitor::visit(FunctionNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);    

    node -> function(static_cast<const OverloadedFunctionSymbol*>(sym));
}

void CheckVisitor::visit(TemplateFunctionNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION);    

    auto ov_func = static_cast<const OverloadedFunctionSymbol*>(sym);

    if ( node -> templateParams().empty() )
        node -> function(ov_func);
    else
    {
        struct ExtractTemplateParam : boost::static_visitor<TemplateParam>
        {
            auto operator()(const std::shared_ptr<ExprNode>& expr) { return TemplateParam(*expr -> getCompileTimeValue()); }
            auto operator()(const TypeInfo& type_info)             { return TemplateParam(type_info); }
        } extract;
        
        std::vector<TemplateParam> template_arguments;
        for ( auto param : node -> templateParams() )
            template_arguments.emplace_back(boost::apply_visitor(extract, param));

        node -> function(new PartiallyInstantiatedFunctionSymbol(ov_func, template_arguments));
    }
}

void CheckVisitor::visit(VariableNode* node)
{
    const auto& template_info = node -> scope -> templateInfo();

    if ( template_info.sym && template_info.isIn(node -> name()) )
    {
        auto replace = template_info.getReplacement(node -> name());

        if ( replace -> which() == 0 )
            throw SemanticError(node -> name() + " is typename");

        node -> getNum() = std::make_unique<NumberNode>(std::to_string(boost::get<int>(*replace)));
        node -> getNum() -> scope = node -> scope;
        node -> getNum() -> build_scope();

        node -> getNum() -> accept(*this);
        return;
    }
    
    auto sym = node -> scope -> resolve(node -> name());
    node -> variable(static_cast<const VariableSymbol*>(sym));
}

void CheckVisitor::visit(CallNode* node)
{
    visitChildren(node);
    node -> checkCall();
}

void CheckVisitor::visit(ReturnNode* node)
{
    node -> expr() -> accept(*this);

    if ( node -> isInInlineCall() )
        return;

    if ( function_scopes.empty() )
        throw SemanticError("return is not in a function");

    node -> function(function_scopes.top() -> func);

    auto unqualified_type = node -> expr() -> getType().unqualified();
    if ( unqualified_type -> getTypeKind() != TypeKind::POINTER )
    {
        auto ov_func = static_cast<const ObjectType*>(unqualified_type) -> resolveMethod(unqualified_type -> getName());
        ov_func -> resolveCall({valueOf(node -> expr())});
    }
}

void CheckVisitor::visit(UnsafeBlockNode* node)
{
    node -> block() -> accept(*this);
}

void CheckVisitor::visit(VarInferTypeDeclarationNode* node)
{
    auto type = node -> expr() -> getType().unqualified();
    assert(type -> isObjectType());

    auto ov_func = static_cast<const ObjectType*>(type) -> resolveMethod(type -> getName());
    node -> callInfo(ov_func -> resolveCall({valueOf(node -> expr())}));
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

void CheckVisitor::visit(FunctionDeclarationNode* node) 
{ 
    function_scopes.push(node -> functionScope());
    visitChildren(node); 
    function_scopes.pop();
}

void CheckVisitor::visit(IfNode *node)        { visitChildren(node); }
void CheckVisitor::visit(ForNode *node)       { visitChildren(node); }
void CheckVisitor::visit(WhileNode *node)     { visitChildren(node); }
void CheckVisitor::visit(StatementNode *node) { visitChildren(node); }

void CheckVisitor::visit(NullNode*) { } 
void CheckVisitor::visit(BreakNode* ) { }
void CheckVisitor::visit(NumberNode *) { }
void CheckVisitor::visit(StringNode *) { }
void CheckVisitor::visit(ExternNode *) { }
void CheckVisitor::visit(ImportNode* ) { }
