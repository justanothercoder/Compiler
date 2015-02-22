#include "genssavisitor.hpp"
#include "ifnode.hpp"
#include "fornode.hpp"
#include "dotnode.hpp"
#include "callnode.hpp"
#include "addrnode.hpp"
#include "unarynode.hpp"
#include "whilenode.hpp"
#include "stringnode.hpp"
#include "importnode.hpp"
#include "returnnode.hpp"
#include "externnode.hpp"
#include "numbernode.hpp"
#include "bracketnode.hpp"
#include "functionnode.hpp"
#include "variablenode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"
#include "comp.hpp"
#include "builtins.hpp"
#include "localscope.hpp"
#include "typefactory.hpp"
#include "globalconfig.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"

#include "newcommand.hpp"
#include "elemcommand.hpp"
#include "gotocommand.hpp"
#include "callcommand.hpp"
#include "paramcommand.hpp"
#include "labelcommand.hpp"
#include "returncommand.hpp"
#include "assigncommand.hpp"
#include "unaryopcommand.hpp"
#include "iffalsecommand.hpp"
#include "binaryopcommand.hpp"
#include "assignrefcommand.hpp"

#include "dotarg.hpp"
#include "numberarg.hpp"
#include "stringarg.hpp"
#include "variablearg.hpp"

#include "logger.hpp"

GenSSAVisitor::GenSSAVisitor(ThreeAddressCode& code) : code(code)
{
    auto str_type = static_cast<const StructSymbol*>(BuiltIns::ASCII_string_type);

    for ( auto func : BuiltIns::global_scope -> getFunctions() )
    {
        if ( func -> getName() == "putchar" || func -> getName() == "getchar" )
            code.addExternalFunction(func);
    }

    for ( auto func : str_type -> methods() )
        code.addExternalFunction(func);

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
        generateParam(*arg, info);
        params_size += info.desired_type -> sizeOf();
    }

    _arg = code.add(std::make_shared<CallCommand>(call_info.callee, params_size));
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
    auto expr = getArg(node -> expr());
    auto base = getArg(node -> base());

    auto base_type = node -> base() -> getType().unqualified();

    if ( base_type -> isArray() ) {
        _arg = code.add(std::make_shared<ElemCommand>(base, expr));
    }
    else if ( base_type == BuiltIns::ASCII_string_type ) {
        _arg = code.add(std::make_shared<ElemCommand>(base, expr, true));
    }
    else {
        generateCall({base, expr}, node -> callInfo(), node -> inlineInfo());
    }
}

void GenSSAVisitor::visit(UnaryNode* node)
{
    generateCall({getArg(node -> expr())}, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(NewExpressionNode* node)
{
    auto expr_type = node -> getType().unqualified();
    auto tmp_obj = code.add(std::make_shared<NewCommand>(expr_type));
    
    if ( isIntType(expr_type) || isCharType(expr_type) )
    {
        bool is_char_assign = isCharType(expr_type);

        if ( node -> params().empty() )
        {
            code.addConst(0);
            code.add(std::make_shared<AssignCommand>(tmp_obj, std::make_shared<NumberArg>(0), is_char_assign));
        }
        else
        {
            code.add(std::make_shared<AssignCommand>(tmp_obj, getArg(node -> params()[0].get()), is_char_assign));
        }
    }
    else
    {
        auto arguments = std::vector<Argument>{tmp_obj};

        for ( const auto& param : node -> params() )
            arguments.push_back(getArg(param.get()));

        generateCall(arguments, node -> callInfo(), node -> inlineInfo());
    }

    _arg = tmp_obj;
}

void GenSSAVisitor::visit(BinaryOperatorNode* node)
{
    auto lhs_type = node -> lhs() -> getType();
    auto rhs_type = node -> rhs() -> getType();

    auto rhs = getArg(node -> rhs());
    auto lhs = getArg(node -> lhs());

    if ( isSimpleType(lhs_type.unqualified()) && isSimpleType(rhs_type.unqualified()) )
    {
        if ( node -> op() == BinaryOp::ASSIGN ) {
            code.add(std::make_shared<AssignCommand>(lhs, rhs, isCharType(lhs_type.unqualified())));
        }
        else {
            _arg = code.add(std::make_shared<BinaryOpCommand>(node -> op(), lhs, rhs));
        }
    }
    else {
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
    auto function = static_cast<const FunctionalSymbol*>(node -> getDefinedSymbol());
    auto scope_name = function -> getScopedTypedName();

    code.newBlock(function -> innerScope(), scope_name);
    code.add(std::make_shared<LabelCommand>(code.newLabel(scope_name)));

    for ( auto param : node -> paramsSymbols() )
        code.rememberVar(param);

    node -> body() -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode* node)
{
    auto variable = static_cast<const VarSymbol*>(node -> getDefinedSymbol());
    code.rememberVar(variable);

    if ( !node -> isField() )
    {
        auto var_type = variable -> typeOf();
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
                
            auto args = std::vector<Argument>{var_arg};

            for ( const auto& param : node -> constructorParams() )
                args.push_back(getArg(param.get()));

            generateCall(args, node -> callInfo(), node -> inlineInfo()); 
        }
    }
}

void GenSSAVisitor::visit(AddrNode* node)
{
    _arg = code.add(std::make_shared<UnaryOpCommand>(node -> op(), getArg(node -> expr())));
}

void GenSSAVisitor::visit(NullNode* )
{
    code.addConst(0);
    _arg = std::make_shared<NumberArg>(0);
}

void GenSSAVisitor::visit(DotNode *node)
{
    if ( node -> member() -> isFunction() ) {
        _arg = getArg(node -> base());
    }
    else
    {
        auto var = static_cast<const VarSymbol*>(node -> member());
        _arg = std::make_shared<DotArg>(getArg(node -> base()), node -> baseType() -> offsetOf(var -> getName()), var);
    }
}

void GenSSAVisitor::visit(StatementNode *node)
{
    for ( const auto& stat : node -> statements() )
        stat -> accept(*this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
    if ( node -> variable() -> isField() )
    {
        auto this_var = node -> scope -> resolveVariable("this");
        auto offset = static_cast<const ObjectType*>(this_var -> typeOf().unqualified()) -> offsetOf(node -> name());
        _arg = std::make_shared<DotArg>(std::make_shared<VariableArg>(this_var), offset, node -> variable());
    }
    else
    {
        _arg = std::make_shared<VariableArg>(node -> variable());
    }
}

void GenSSAVisitor::visit(StringNode* node)
{
    code.addString(node -> str());
    _arg = std::make_shared<StringArg>(node -> str());
}

void GenSSAVisitor::visit(NumberNode* node)
{
    auto num = std::stoi(node -> num());

    code.addConst(num);
    _arg = std::make_shared<NumberArg>(num);
}

void GenSSAVisitor::visit(CallNode* node)
{
    auto args = std::vector<Argument>{ };
    
    if ( node -> callInfo().callee -> isMethod() )
        args.push_back(getArg(node -> caller()));
   
    for ( const auto& param : node -> params() )
        args.push_back(getArg(param.get()));

    generateCall(args, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(ReturnNode* node)
{
    auto expr_arg = getArg(node -> expr());

    if ( node -> isInInlineCall() )
    {
        auto expr_type = node -> expr() -> getType();
        auto var_arg = std::make_shared<VariableArg>(node -> scope -> resolveVariable("$"));

        if ( node -> function() -> type().returnType().isReference() )
        {
            code.add(std::make_shared<AssignRefCommand>(var_arg, expr_arg));
        }
        else if ( isSimpleType(expr_type.base()) )
        {
            code.add(std::make_shared<AssignCommand>(var_arg, expr_arg, isCharType(expr_type.base())));
        }
        else
        {
            auto copy_constructor = static_cast<const StructSymbol*>(expr_type.base()) -> getCopyConstructor();
           
            generateParam(expr_arg, ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base())));
            generateParam(var_arg , ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base())));
            _arg = code.add(std::make_shared<CallCommand>(copy_constructor, 2 * Comp::config().int_size));
        }
        _arg = var_arg;
        code.add(std::make_shared<GotoCommand>(loop_label.top().second));
        return;
    }

    code.add(std::make_shared<ReturnCommand>(expr_arg, node -> function() -> type().returnType().isReference()));
}

void GenSSAVisitor::visit(UnsafeBlockNode* node)
{
    node -> block() -> accept(*this);
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode* node)
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
   
    generateCall({var, getArg(node -> expr())}, node -> callInfo(), node -> inlineInfo());
}

void GenSSAVisitor::visit(TemplateStructDeclarationNode* node)
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
        auto _this_var = node -> scope -> resolveVariable("this");
        _arg = std::make_shared<VariableArg>(_this_var);
    }
}

void GenSSAVisitor::visit(ModuleNode* ) { }
void GenSSAVisitor::visit(ModuleMemberAccessNode* ) { }
void GenSSAVisitor::visit(ImportNode *) { }
void GenSSAVisitor::visit(TemplateFunctionNode* ) { }

void GenSSAVisitor::genInlineCall(const InlineInfo& inline_info, std::vector<Argument> params)
{
    assert(inline_info.function_body);

    auto exit_from_function_label = code.newLabel();

    loop_label.emplace(nullptr, exit_from_function_label);

    auto param_it = std::begin(params);

    auto return_var = inline_info.function_body -> scope -> resolveVariable("$");
    code.rememberVar(return_var);

    for ( const auto& var : inline_info.locals )
    {
        code.rememberVar(var);
            
        auto var_type = var -> typeOf();
        auto var_arg = std::make_shared<VariableArg>(var);

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
            auto copy_constructor = static_cast<const StructSymbol*>(var_type.base()) -> getCopyConstructor();

            generateParam(*param_it, ConversionInfo(nullptr, TypeFactory::getReference(var_type.base())));
            generateParam(var_arg  , ConversionInfo(nullptr, TypeFactory::getReference(var_type.base())));
            _arg = code.add(std::make_shared<CallCommand>(copy_constructor, 2 * Comp::config().int_size));
        }

        ++param_it;
    }

    inline_info.function_body -> accept(*this);
    code.add(std::make_shared<LabelCommand>(exit_from_function_label));

    _arg = std::make_shared<VariableArg>(return_var);

    loop_label.pop();
}

void GenSSAVisitor::generateParam(Argument arg, ConversionInfo info) { code.add(std::make_shared<ParamCommand>(arg, info)); }

