#include "genssavisitor.hpp"
#include "numbernode.hpp"
#include "binaryoperatornode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "callnode.hpp"
#include "functiondeclarationnode.hpp"
#include "structdeclarationnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "addrnode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "dotnode.hpp"
#include "stringnode.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"
#include "nullnode.hpp"
#include "variabledeclarationnode.hpp"
#include "newexpressionnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "importnode.hpp"
#include "functionnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "externnode.hpp"
#include "typefactory.hpp"
#include "localscope.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

#include "iffalsecommand.hpp"
#include "gotocommand.hpp"
#include "labelcommand.hpp"
#include "elemcommand.hpp"
#include "newcommand.hpp"
#include "assigncommand.hpp"
#include "paramcommand.hpp"
#include "binaryopcommand.hpp"
#include "unaryopcommand.hpp"
#include "returncommand.hpp"
#include "callcommand.hpp"
#include "assignrefcommand.hpp"

#include "numberarg.hpp"
#include "variablearg.hpp"
#include "stringarg.hpp"
#include "dotarg.hpp"

#include "logger.hpp"

GenSSAVisitor::GenSSAVisitor(ThreeAddressCode& code) : code(code)
{
    auto str_type = static_cast<StructSymbol*>(BuiltIns::ASCII_string_type.get());

    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(BuiltIns::global_scope -> resolve("putchar")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(BuiltIns::global_scope -> resolve("print")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("length")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("operator+")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("string")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    for ( const auto& func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("operator=")) -> getTypeInfo().symbols )
        code.addExternalFunction(static_cast<const FunctionSymbol*>(func.second.get()));
    
    code.newBlock(BuiltIns::global_scope.get());
}

const std::shared_ptr<Arg>& GenSSAVisitor::getArg(AST* node)
{
    node -> accept(*this);
    return _arg;
}

void GenSSAVisitor::generateCall(std::vector<Argument> args, const CallInfo& call_info, const InlineInfo& inline_info)
{
    if ( inline_info.function_body )
    {
        genInlineCall(inline_info, args);
        return;
    }

    auto params_size = 0;
    
    auto it = call_info.conversions.rbegin();

    for ( auto arg = args.rbegin(); arg != args.rend(); ++arg, ++it )
    {
        auto info = *it;
        code.add(std::make_shared<ParamCommand>(*arg, info));
        params_size += info.desired_type -> sizeOf();
    }

    genCall(call_info.callee, params_size);
}

void GenSSAVisitor::visit(ExternNode* node) { code.addExternalFunction(static_cast<const FunctionSymbol*>(node -> getDefinedSymbol())); }

void GenSSAVisitor::visit(IfNode* node)
{
    auto false_label = code.newLabel();
    auto exit_label  = code.newLabel();

    auto expr = getArg(node -> condition());

    code.add(std::make_shared<IfFalseCommand>(expr, false_label));

    node -> trueBranch() -> accept(*this);

    code.add(std::make_shared<GotoCommand>(exit_label));
    code.add(std::make_shared<LabelCommand>(false_label));

    node -> falseBranch() -> accept(*this);

    code.add(std::make_shared<LabelCommand>(exit_label));
}

void GenSSAVisitor::visit(ForNode* node)
{
    auto cycle_label = code.newLabel();
    auto exit_label  = code.newLabel();

    loop_label.emplace(cycle_label, exit_label);

    node -> initializer() -> accept(*this);
    code.add(std::make_shared<LabelCommand>(cycle_label));

    auto cond = getArg(node -> condition());

    code.add(std::make_shared<IfFalseCommand>(cond, exit_label));

    node -> body() -> accept(*this);
    node -> iteration() -> accept(*this);

    loop_label.pop();

    code.add(std::make_shared<GotoCommand>(cycle_label));
    code.add(std::make_shared<LabelCommand>(exit_label));
}

void GenSSAVisitor::visit(WhileNode* node)
{
    auto exit_label  = code.newLabel();
    auto cycle_label = code.newLabel();

    loop_label.emplace(cycle_label, exit_label);

    code.add(std::make_shared<LabelCommand>(cycle_label));

    auto cond = getArg(node -> condition());

    code.add(std::make_shared<IfFalseCommand>(cond, exit_label));

    node -> body() -> accept(*this);

    loop_label.pop();

    code.add(std::make_shared<GotoCommand>(cycle_label));
    code.add(std::make_shared<LabelCommand>(exit_label));
}

void GenSSAVisitor::visit(BracketNode* node)
{
    if ( node -> base() -> getType().unqualified() -> getTypeKind() == TypeKind::ARRAY )
    {
        auto expr = getArg(node -> expr());
        auto base = getArg(node -> base());

        _arg = code.add(std::make_shared<ElemCommand>(base, expr));
        return;
    }
    
    if ( node -> base() -> getType().unqualified() == BuiltIns::ASCII_string_type.get() )
    {
        auto expr = getArg(node -> expr());
        auto base = getArg(node -> base());

        _arg = code.add(std::make_shared<ElemCommand>(base, expr, true));
        return;
    }

    auto expr = getArg(node -> expr());
    auto base = getArg(node -> base());

    generateCall({base, expr}, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(UnaryNode* node)
{
    generateCall({getArg(node -> expr())}, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(NewExpressionNode* node)
{
    const auto& params = node -> params();

    auto expr_type = node -> getType().unqualified();

    auto tmp_obj = code.add(std::make_shared<NewCommand>(expr_type));
    
    if ( isIntType(expr_type) || isCharType(expr_type) )
    {
        bool is_char_assign = isCharType(expr_type);

        if ( params.empty() )
        {
            code.addConst(0);
            code.add(std::make_shared<AssignCommand>(tmp_obj, std::make_shared<NumberArg>(0), is_char_assign));
        }
        else
        {
            code.add(std::make_shared<AssignCommand>(tmp_obj, getArg(params[0].get()), is_char_assign));
        }

        _arg = tmp_obj;
        return;
    }
    
    auto params_size = 0;

    for ( auto param = params.rbegin(); param != params.rend(); ++param )
    {
        auto info = *(node -> callInfo().conversions.rbegin() + (param - params.rbegin()));
        genParam(param -> get(), info);
        params_size += info.desired_type -> sizeOf();
    }

    code.add(std::make_shared<ParamCommand>(tmp_obj, ConversionInfo(nullptr, TypeFactory::getReference(expr_type))));

    params_size += Comp::config().int_size;

    genCall(node -> callInfo().callee, params_size);
    _arg = tmp_obj;
}

void GenSSAVisitor::visit(BinaryOperatorNode* node)
{
    auto lhs_type = node -> lhs() -> getType();
    auto rhs_type = node -> rhs() -> getType();

    if ( isSimpleType(lhs_type.unqualified()) && isSimpleType(rhs_type.unqualified()) )
    {
        auto rhs = getArg(node -> rhs());
        auto lhs = getArg(node -> lhs());

        if ( node -> op() == BinaryOp::ASSIGN )
            code.add(std::make_shared<AssignCommand>(lhs, rhs, isCharType(lhs_type.unqualified())));
        else
            _arg = code.add(std::make_shared<BinaryOpCommand>(node -> op(), lhs, rhs));
    }
    else
    {
        auto rhs = getArg(node -> rhs());
        auto lhs = getArg(node -> lhs());

        generateCall({lhs, rhs}, node -> callInfo(), node -> inlineInfo());
    }
}

void GenSSAVisitor::visit(StructDeclarationNode* node)
{
    for ( auto decl : node -> getChildren() )
        decl -> accept(*this);
}

void GenSSAVisitor::visit(FunctionDeclarationNode* node)
{
    auto function = static_cast<const FunctionSymbol*>(node -> getDefinedSymbol());
    auto scope_name = function -> getScopedTypedName();

    code.newBlock(function -> getScope(), scope_name);
    code.add(std::make_shared<LabelCommand>(code.newLabel(scope_name)));

    for ( auto param : node -> paramsSymbols() )
        code.rememberVar(param.get());

    node -> body() -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode* node)
{
    auto variable = static_cast<const VariableSymbol*>(node -> getDefinedSymbol());
    code.rememberVar(variable);

    if ( !node -> isField() )
    {
        auto var_type = variable -> getType();
        auto var_arg = std::make_shared<VariableArg>(variable);

        if ( node -> typeInfo().isRef() || (node -> typeInfo().modifiers().size() > 0 && !node -> constructorParams().empty()) )
        {
            auto arg = getArg(node -> constructorParams()[0].get());
            code.add(std::make_shared<AssignCommand>(var_arg, arg, false));
        }
        else
        {
            if ( isIntType(var_type.unqualified()) || isCharType(var_type.unqualified()) )
            {
                bool is_char_assign = isCharType(var_type.unqualified());
                if ( node -> constructorParams().empty() )
                {
                    code.addConst(0);
                    code.add(std::make_shared<AssignCommand>(var_arg, std::make_shared<NumberArg>(0), is_char_assign));
                }
                else
                {
                    code.add(std::make_shared<AssignCommand>(var_arg, getArg(node -> constructorParams()[0].get()), is_char_assign));
                }
                return;
            }

            if ( node -> inlineInfo().function_body )
            {
                auto args = std::vector<Argument>{var_arg};

                for ( const auto& param : node -> constructorParams() ) {
                    args.push_back(getArg(param.get()));
                }                

                genInlineCall(node -> inlineInfo(), args);
                return;
            }

            const auto& params = node -> constructorParams();
            auto params_size = 0;

            for ( auto param = params.rbegin(); param != params.rend(); ++param )
            {
                auto info = *(node -> callInfo().conversions.rbegin() + (param - params.rbegin()));
                genParam(param -> get(), info);

                params_size += info.desired_type -> sizeOf();
            }

            auto variable = static_cast<const VariableSymbol*>(node -> getDefinedSymbol());
            code.add(std::make_shared<ParamCommand>(var_arg, ConversionInfo(nullptr, TypeFactory::getReference(variable -> getType().base()))));

            params_size += Comp::config().int_size;
            genCall(node -> callInfo().callee, params_size);
        }
    }
}

void GenSSAVisitor::visit(AddrNode *node)
{
    _arg = code.add(std::make_shared<UnaryOpCommand>(node -> op(), getArg(node -> expr())));
}

void GenSSAVisitor::visit(NullNode *)
{
    code.addConst(0);
    _arg = std::make_shared<NumberArg>(0);
}

void GenSSAVisitor::visit(DotNode *node)
{
    if ( node -> member() -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION )
    {
        _arg = getArg(node -> base());
    }
    else
    {
        auto var = static_cast<const VariableSymbol*>(node -> member());
        _arg = std::make_shared<DotArg>(getArg(node -> base()), node -> baseType() -> offsetOf(var), var);
    }
}

void GenSSAVisitor::visit(StatementNode *node)
{
    for ( const auto& stat : node -> statements() )
        stat -> accept(*this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
    if ( node -> isTemplateParam() )
    {
        node -> getNum() -> accept(*this);
        return;
    }

    if ( node -> variable() -> isField() )
    {
        auto this_var = static_cast<const VariableSymbol*>(node -> scope -> resolve("this"));
        auto offset = static_cast<const ObjectType*>(this_var -> getType().unqualified()) -> offsetOf(node -> variable());
        _arg = std::make_shared<DotArg>(std::make_shared<VariableArg>(this_var), offset, node -> variable());
    }
    else
    {
        _arg = std::make_shared<VariableArg>(node -> variable());
    }
}

void GenSSAVisitor::visit(StringNode* node)
{
    code.addString(node -> getStr());
    _arg = std::make_shared<StringArg>(node -> getStr());
}

void GenSSAVisitor::visit(NumberNode* node)
{
    auto num = std::stoi(node -> getNum());

    code.addConst(num);
    _arg = std::make_shared<NumberArg>(num);
}

void GenSSAVisitor::visit(CallNode* node)
{
    auto args = std::vector<Argument>{ };    
        
    const auto& params = node -> params();

    for ( auto param = params.rbegin(); param != params.rend(); ++param )
        args.push_back(getArg(param -> get()));

    if ( node -> callInfo().callee -> isMethod() )
        args.push_back(getArg(node -> caller()));

    std::reverse(std::begin(args), std::end(args));

    generateCall(args, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(ReturnNode* node)
{
    auto expr_arg = getArg(node -> expr());

    if ( node -> isInInlineCall() )
    {
        auto expr_type = node -> expr() -> getType();
        auto var_arg = std::make_shared<VariableArg>(static_cast<const VariableSymbol*>(node -> scope -> resolve("$")));

        if ( expr_type.isReference() )
        {
            code.add(std::make_shared<AssignRefCommand>(var_arg, expr_arg));
        }
        else if ( isSimpleType(expr_type.base()) )
        {
            code.add(std::make_shared<AssignCommand>(var_arg, expr_arg, isCharType(expr_type.base())));
        }
        else
        {
            code.add(std::make_shared<ParamCommand>(expr_arg, ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base()))));
            code.add(std::make_shared<ParamCommand>(var_arg , ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base()))));
            genCall(static_cast<const StructSymbol*>(expr_type.base()) -> getCopyConstructor(), 2 * Comp::config().int_size);
        }
        _arg = var_arg;
        code.add(std::make_shared<GotoCommand>(loop_label.top().second));
        return;
    }

    code.add(std::make_shared<ReturnCommand>(expr_arg, node -> function() -> type().returnType().isReference()));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
    node -> block() -> accept(*this);
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto variable = static_cast<const VariableSymbol*>(node -> getDefinedSymbol());
    code.rememberVar(variable);

    auto expr_type = node -> expr() -> getType().unqualified();
    auto var = std::make_shared<VariableArg>(variable);

    if ( isIntType(expr_type) || isCharType(expr_type) )
    {
        code.add(std::make_shared<AssignCommand>(var, getArg(node -> expr()), isCharType(expr_type)));
        return;
    }
    
    genParam(node -> expr(), node -> callInfo().conversions[0]);
    code.add(std::make_shared<ParamCommand>(var, ConversionInfo(nullptr, TypeFactory::getReference(expr_type))));
    genCall(node -> callInfo().callee, expr_type -> sizeOf() + Comp::config().int_size);
}

void GenSSAVisitor::visit(TemplateStructDeclarationNode *node)
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}
    
void GenSSAVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    for ( auto instance : node -> allInstances() )
        instance -> accept(*this);
}

void GenSSAVisitor::visit(BreakNode* )
{
    code.add(std::make_shared<GotoCommand>(loop_label.top().second));
}

void GenSSAVisitor::visit(FunctionNode* node)
{
    if ( node -> function() -> isMethod() )
    {
        auto _this_var = static_cast<const VariableSymbol*>(node -> scope -> resolve("this"));
        _arg = std::make_shared<VariableArg>(_this_var);
    }
}

void GenSSAVisitor::visit(ModuleNode* ) { }
void GenSSAVisitor::visit(TypeNode* ) { }
void GenSSAVisitor::visit(ModuleMemberAccessNode* ) { }
void GenSSAVisitor::visit(ImportNode *) { }
void GenSSAVisitor::visit(TemplateFunctionNode* ) { }

void GenSSAVisitor::genParam(ExprNode* node, ConversionInfo conversion_info)
{
    code.add(std::make_shared<ParamCommand>(getArg(node), conversion_info));
}

void GenSSAVisitor::genCall(const FunctionSymbol* func, int params_size)
{
    _arg = code.add(std::make_shared<CallCommand>(func, params_size));
}

void GenSSAVisitor::genInlineCall(const InlineInfo& inline_info, std::vector<Argument> params)
{
    assert(inline_info.function_body);

    auto exit_from_function_label = code.newLabel();

    loop_label.emplace(nullptr, exit_from_function_label);

    auto param_it = std::begin(params);

    auto return_var = static_cast<const VariableSymbol*>(inline_info.function_body -> scope -> resolve("$"));
    code.rememberVar(return_var);

    for ( auto var : inline_info.locals )
    {
        code.rememberVar(var.get());
            
        auto var_type = var -> getType();
        auto var_arg = std::make_shared<VariableArg>(var.get());

        if ( var_type.isReference() )
        {
            code.add(std::make_shared<AssignRefCommand>(var_arg, *param_it));
        }
        else if ( isSimpleType(var_type.base()) )
        {
            code.add(std::make_shared<AssignCommand>(var_arg, *param_it, isCharType(var_type.base())));
        }
        else
        {
            code.add(std::make_shared<ParamCommand>(*param_it, ConversionInfo(nullptr, TypeFactory::getReference(var_type.base()))));
            code.add(std::make_shared<ParamCommand>(var_arg  , ConversionInfo(nullptr, TypeFactory::getReference(var_type.base()))));
            genCall(static_cast<const StructSymbol*>(var_type.base()) -> getCopyConstructor(), 2 * Comp::config().int_size);
        }

        ++param_it;
    }

    inline_info.function_body -> accept(*this);
    code.add(std::make_shared<LabelCommand>(exit_from_function_label));

    _arg = std::make_shared<VariableArg>(return_var);

    loop_label.pop();
}

bool GenSSAVisitor::isIntType(const Type* t)    { return t == BuiltIns::int_type.get(); }
bool GenSSAVisitor::isCharType(const Type* t)   { return t == BuiltIns::char_type.get(); }
bool GenSSAVisitor::isPointer(const Type* t)    { return t -> getTypeKind() == TypeKind::POINTER; }
bool GenSSAVisitor::isReference(const Type* t)  { return t -> isReference(); }
bool GenSSAVisitor::isSimpleType(const Type* t) { return isIntType(t) || isCharType(t) || isPointer(t) || isReference(t); }
