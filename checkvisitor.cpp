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
#include "lambdanode.hpp"
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
#include "modulesymbol.hpp"
#include "varsymbol.hpp"
#include "templatestructsymbol.hpp"
#include "templatefunctionsymbol.hpp"
#include "comp.hpp"
#include "builtins.hpp"
#include "compilableunit.hpp"
#include "noviableoverloaderror.hpp"
#include "typefactory.hpp"
#include "logger.hpp"

const FunctionTypeInfo& CheckVisitor::getCallArguments()
{
    if ( arguments_stack.empty() )
        throw SemanticError("Internal error");
    return arguments_stack.top();
}

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

void CheckVisitor::visit(BinaryOperatorNode* node)
{
    visitChildren(node);
    node -> callInfo(checkCall(node -> function(), node -> arguments()));
}

void CheckVisitor::visit(NewExpressionNode* node)
{
    for ( auto param : node -> typeInfo().templateArgumentsInfo() )
    {
        if ( param.which() == 0 ) {
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
        }
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
        if ( param.which() == 0 ) {
            boost::get< std::shared_ptr<ExprNode> >(param) -> accept(*this);
        }
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
            auto constructor = struct_symbol -> resolveMethod(struct_symbol -> typeName(), types);

            if ( constructor == nullptr ) {
                throw SemanticError("No constructor defined");
            }

            node -> callInfo(checkCall(constructor, arguments));
        }
    }
}

void CheckVisitor::visit(AddrNode* node)
{
    node -> expr() -> accept(*this);

    if ( node -> op() == AddrOp::REF )
    {
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

    auto base_type = node -> base() -> getType().unqualified();
    if ( !base_type -> isObjectType()  )
        throw SemanticError("'" + node -> base() -> toString() + "' is not an instance of struct.");

    node -> baseType(static_cast<const ObjectType*>(base_type));

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

    node -> memberSymbol(module_sym -> resolve(node -> member()));
}

void CheckVisitor::visit(ModuleNode* node)
{
    auto sym = node -> scope -> resolve(node -> name());
    assert(sym && sym -> isModule());

    node -> module(static_cast<const ModuleSymbol*>(sym));
}

void CheckVisitor::visit(FunctionNode* node)
{
    try { getCallArguments(); }
    catch ( SemanticError& e ) { throw SemanticError("No arguments provided to '" + node -> name() + "'"); }

    auto sym = node -> scope -> resolveFunction(node -> name(), getCallArguments());

    if ( sym == nullptr )
        throw NoViableOverloadError(node -> name(), getCallArguments().params());

    node -> function(sym);
}

void CheckVisitor::visit(TemplateFunctionNode* node)
{
    auto sym = node -> scope -> resolveTemplateFunction(node -> name(), getTemplateArguments(node -> templateArgumentsInfo()), getCallArguments());

    if ( sym == nullptr )
        throw NoViableOverloadError(node -> name(), getCallArguments().params());

    node -> function(sym);
}

void CheckVisitor::visit(VariableNode* node)
{
    auto sym = node -> scope -> resolveVariable(node -> name());
    assert(sym != nullptr);
    node -> variable(sym);
}

void CheckVisitor::visit(CallNode* node)
{
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

    Logger::log(node -> toString());
    Logger::log(node -> expr() -> getType().getName());

    if ( node -> isInInlineCall() )
        return;

    if ( function_scopes.empty() )
        throw SemanticError("return is not in a function");

    auto enclosing_function = function_scopes.top() -> func;
    node -> function(enclosing_function);

    auto unqualified_type = node -> expr() -> getType().unqualified();
    if ( unqualified_type -> isObjectType() )
    {
        auto obj_type = static_cast<const ObjectType*>(unqualified_type);
        auto ref_to_obj_type = TypeFactory::getReference(obj_type);
//        auto copy_constr = obj_type -> resolveMethod(obj_type -> typeName(), {VariableType(TypeFactory::getReference(obj_type), true)});
        auto copy_constr = obj_type -> methodWith(obj_type -> typeName(), {ref_to_obj_type, VariableType(ref_to_obj_type, true)});

        if ( copy_constr == nullptr )
            throw SemanticError("Cannot initialize return value of type '" + enclosing_function -> type().returnType().getName() + "' with value of type '" + obj_type -> typeName() + "'");

        checkCall(copy_constr, {valueOf(node -> expr())});
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

    auto ov_func = static_cast<const ObjectType*>(type) -> resolveMethod(type -> typeName(), {TypeFactory::getReference(type)});
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

void CheckVisitor::visit(LambdaNode* node)
{
    /* TODO add captured variables check */
    function_scopes.push(static_cast<FunctionScope*>(node -> callOp() -> innerScope()));
    node -> body() -> accept(*this);
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
