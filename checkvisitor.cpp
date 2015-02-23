#include "checkvisitor.hpp"
#include "ifnode.hpp"
#include "dotnode.hpp"
#include "fornode.hpp"
#include "addrnode.hpp"
#include "callnode.hpp"
#include "unarynode.hpp"
#include "whilenode.hpp"
#include "importnode.hpp"
#include "modulenode.hpp"
#include "returnnode.hpp"
#include "bracketnode.hpp"
#include "variablenode.hpp"
#include "functionnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "templatefunctionnode.hpp"
#include "structdeclarationnode.hpp"
#include "modulememberaccessnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "objecttype.hpp"
#include "structsymbol.hpp"
#include "modulesymbol.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "templatefunctionsymbol.hpp"
#include "partiallyinstantiatedfunctionsymbol.hpp"
#include "comp.hpp"
#include "builtins.hpp"
#include "compilableunit.hpp"
#include "noviableoverloaderror.hpp"
#include "typefactory.hpp"
#include "logger.hpp"

const FunctionTypeInfo& CheckVisitor::getCallArguments() { return arguments_stack.top(); }

void CheckVisitor::pushArguments(FunctionTypeInfo info) { arguments_stack.push(info); }
void CheckVisitor::popArguments() { arguments_stack.pop(); }

void CheckVisitor::visitChildren(AST* node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

std::vector<ValueInfo> CheckVisitor::extractArguments(const std::vector< std::unique_ptr<ExprNode> >& params)
{
    auto result = std::vector<ValueInfo>{ };

    for ( const auto& param : params )
        result.emplace_back(param -> getType(), param -> isLeftValue());

    return result;
}

CallInfo CheckVisitor::checkCall(const FunctionalType* function, std::vector<ValueInfo> arguments)
{
    assert(function != nullptr);
    try
    {
        return function -> resolveCall(arguments);
    }
    catch ( NoViableOverloadError& e ) { throw NoViableOverloadError(function -> typeName(), arguments); }
}

void CheckVisitor::visit(BracketNode* node)
{
    visitChildren(node);
    node -> callInfo(checkCall(node -> function(), node -> arguments()));
}

void CheckVisitor::visit(UnaryNode* node)
{
    visitChildren(node);
    node -> callInfo(checkCall(node -> function(), node -> arguments()));
}

void CheckVisitor::visit(NewExpressionNode* node)
{
    for ( auto param : node -> typeInfo().templateArgumentsInfo() )
    {
        if ( param.which() == 0 )
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
    }
    
    auto tp = fromTypeInfo(node -> typeInfo(), node -> scope.get()).base();
    assert(tp -> isObjectType());
    auto type = static_cast<const ObjectType*>(tp);

    auto types = std::vector<VariableType>{ };
    for ( const auto& param : node -> params() )
    {
        param -> accept(*this);
        types.emplace_back(param -> getType());
    }

    auto arguments = extractArguments(node -> params());
    auto ov_func = type -> resolveMethod(type -> typeName(), types);

    node -> callInfo(checkCall(ov_func, arguments));
}

void CheckVisitor::visit(BinaryOperatorNode* node)
{
    visitChildren(node);

    Logger::log("Processing node " + node -> toString());
    Logger::log("Function: " + node -> function() -> getName());
    Logger::log(std::string("Function: ") + (node -> function() -> isMethod() ? std::string("method") : std::string("not method")));

    node -> callInfo(checkCall(node -> function(), node -> arguments()));
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
    for ( auto param : node -> typeInfo().templateArgumentsInfo() )
    {
        if ( param.which() == 0 )
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
    }

    if ( !node -> isField() )
    {
        auto types = std::vector<VariableType>{ };
        for ( const auto& param : node -> constructorParams() )
        {
            param -> accept(*this);
            types.emplace_back(param -> getType());
        }
       
        if ( !node -> typeInfo().isRef() && node -> typeInfo().modifiers().empty() )
        {
            auto var_type = fromTypeInfo(node -> typeInfo(), node -> scope.get());
            assert(var_type.unqualified() -> isObjectType());

            auto struct_symbol = static_cast<const ObjectType*>(var_type.unqualified());

            auto arguments = extractArguments(node -> constructorParams());
            auto ov_func = struct_symbol -> resolveMethod(struct_symbol -> typeName(), types);

            if ( ov_func == nullptr )
                throw SemanticError("No constructor defined");
            
            node -> callInfo(checkCall(ov_func, arguments));
        }
    }
}

void CheckVisitor::visit(AddrNode* node)
{
    node -> expr() -> accept(*this);

    if ( node -> op() == AddrOp::REF ) {
        if ( !node -> expr() -> isLeftValue() ) {
            throw SemanticError("expression is not an lvalue");
        }
    }
    else
    {
        auto type = node -> expr() -> getType().unqualified();

        if ( !type -> isPointer() ) {
            throw SemanticError("Type of " + node -> toString() + " is not a pointer type.");
        }
    }
}

void CheckVisitor::visit(DotNode* node)
{
    node -> base() -> accept(*this);

    auto _base_type = node -> base() -> getType().unqualified();
    if ( !_base_type -> isObjectType()  )
        throw SemanticError("'" + node -> base() -> toString() + "' is not an instance of struct.");

    node -> baseType(static_cast<const ObjectType*>(_base_type));

    auto mem = node -> baseType() -> resolveMember(node -> memberName());
    if ( mem == nullptr )
        mem = node -> baseType() -> resolveMethod(node -> memberName(), getCallArguments());

    if ( mem == nullptr )
        throw SemanticError(node -> memberName() + " is not member of " + node -> baseType() -> typeName());

    node -> member(mem);
}

void CheckVisitor::visit(ModuleMemberAccessNode* node)
{
    auto module_sym = Comp::getUnit(node -> name()) -> module_symbol;
    assert(module_sym && module_sym -> isModule());

    node -> memberSymbol(module_sym -> resolveVariable(node -> member()));
}

void CheckVisitor::visit(ModuleNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> isModule());

    node -> module(static_cast<const ModuleSymbol*>(sym));
}

void CheckVisitor::visit(FunctionNode* node) 
{
    Logger::log("Processing node " + node -> toString());
    Logger::log("Call arguments: " + getCallArguments().toString());

    auto sym = node -> scope -> resolveFunction(node -> name(), getCallArguments());
    node -> function(sym);
}

void CheckVisitor::visit(TemplateFunctionNode* node) 
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> isFunction());
/*
    auto ov_func = static_cast<const OverloadedFunctionSymbol*>(sym);

    if ( node -> templateArgumentsInfo().empty() )
        node -> function(ov_func);
    else
    {
        auto template_arguments = TemplateArguments{ };
        for ( auto argument_info : node -> templateArgumentsInfo() )
            template_arguments.emplace_back(getTemplateArgument(argument_info));

        node -> function(new PartiallyInstantiatedFunctionSymbol(ov_func, template_arguments));
    }
*/    
}

void CheckVisitor::visit(VariableNode* node)
{
    auto sym = node -> scope -> resolveVariable(node -> name());
    node -> variable(sym);
}

void CheckVisitor::visit(CallNode* node)
{
    Logger::log("Processing node " + node -> toString());

    auto types = std::vector<VariableType>{ };
    for ( const auto& param : node -> params() )
    {
        param -> accept(*this);
        types.push_back(param -> getType());
    }

    pushArguments(types);
    node -> caller() -> accept(*this);
    popArguments();

    node -> callInfo(checkCall(node -> function(), node -> arguments()));
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
    if ( unqualified_type -> isObjectType() )
    {
        auto obj_type = static_cast<const ObjectType*>(unqualified_type);
        auto ov_func = obj_type -> resolveMethod(obj_type -> typeName(), {VariableType(TypeFactory::getReference(obj_type), true)});
        checkCall(ov_func, {valueOf(node -> expr())});
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

    auto ov_func = static_cast<const ObjectType*>(type) -> resolveMethod(type -> typeName(), {node -> expr() -> getType()});
    node -> callInfo(checkCall(ov_func, {valueOf(node -> expr())}));
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

void CheckVisitor::visit(IfNode* node)        { visitChildren(node); }
void CheckVisitor::visit(ForNode* node)       { visitChildren(node); }
void CheckVisitor::visit(WhileNode* node)     { visitChildren(node); }
void CheckVisitor::visit(StatementNode* node) { visitChildren(node); }

void CheckVisitor::visit(NullNode*) { } 
void CheckVisitor::visit(BreakNode* ) { }
void CheckVisitor::visit(NumberNode *) { }
void CheckVisitor::visit(StringNode *) { }
void CheckVisitor::visit(ExternNode *) { }
void CheckVisitor::visit(ImportNode* ) { }
