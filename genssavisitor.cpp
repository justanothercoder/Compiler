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

GenSSAVisitor::GenSSAVisitor(ThreeAddressCode& code) : _arg(nullptr), code(code)
{
    auto str_type = static_cast<StructSymbol*>(BuiltIns::ASCII_string_type);

    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(BuiltIns::global_scope -> resolve("putchar")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(BuiltIns::global_scope -> resolve("print")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("length")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("operator+")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("string")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(str_type -> resolve("operator=")) -> getTypeInfo().symbols )
        code.addExternalFunction(func.second);
    
    code.newBlock(*BuiltIns::global_scope);
}

Arg* GenSSAVisitor::getArg(AST *node)
{
    node -> accept(*this);
    return _arg;
}

void GenSSAVisitor::visit(ExternNode *node) { code.addExternalFunction(node -> definedSymbol); }

void GenSSAVisitor::visit(IfNode *node)
{
    auto false_label = code.newLabel();
    auto exit_label  = code.newLabel();

    auto expr = getArg(node -> cond);

    code.add(new IfFalseCommand(expr, false_label));

    node -> stats_true -> accept(*this);

    code.add(new GotoCommand(exit_label));
    code.add(new LabelCommand(false_label));

    node -> stats_false -> accept(*this);

    code.add(new LabelCommand(exit_label));
}

void GenSSAVisitor::visit(ForNode *node)
{
    auto cycle_label = code.newLabel();
    auto exit_label  = code.newLabel();

    loop_label.push(std::make_pair(cycle_label, exit_label));

    node -> init -> accept(*this);
    code.add(new LabelCommand(cycle_label));

    auto cond = getArg(node -> cond);

    code.add(new IfFalseCommand(cond, exit_label));

    node -> stats -> accept(*this);
    node -> step  -> accept(*this);

    loop_label.pop();

    code.add(new GotoCommand(cycle_label));
    code.add(new LabelCommand(exit_label));
}

void GenSSAVisitor::visit(WhileNode *node)
{
    auto exit_label  = code.newLabel();
    auto cycle_label = code.newLabel();

    loop_label.push(std::make_pair(cycle_label, exit_label));

    code.add(new LabelCommand(cycle_label));

    auto cond = getArg(node -> cond);

    code.add(new IfFalseCommand(cond, exit_label));

    node -> stats -> accept(*this);

    loop_label.pop();

    code.add(new GotoCommand(cycle_label));
    code.add(new LabelCommand(exit_label));
}

void GenSSAVisitor::visit(BracketNode *node)
{
    if ( node -> base -> getType().base() -> getTypeKind() == TypeKind::ARRAY )
    {
        _arg = code.add(new ElemCommand(getArg(node -> base), getArg(node -> expr)));
        return;
    }
    
    if ( node -> base -> getType().unqualified() == BuiltIns::ASCII_string_type )
    {
        _arg = code.add(new ElemCommand(getArg(node -> base), getArg(node -> expr), true));
        return;
    }

//    genParam(node -> expr, node -> call_info.conversions.front());
//    genParam(node -> base, ConversionInfo(nullptr, TypeFactory::getReference(node -> base -> getType().base())));

    genParam(node -> expr, *(std::next(std::begin(node -> call_info.conversions))));
    genParam(node -> base, *(std::begin(node -> call_info.conversions)));

    genCall(node -> call_info.callee, node -> expr -> getType().sizeOf() + Comp::config().int_size);
}

void GenSSAVisitor::visit(UnaryNode *node)
{
    genParam(node -> exp, node -> call_info.conversions.front());
    genCall(node -> call_info.callee, node -> exp -> getType().sizeOf());
}

void GenSSAVisitor::visit(NewExpressionNode *node)
{
    const auto& params = node -> params;

    auto expr_type = node -> getType().unqualified();

    auto tmp_obj = code.add(new NewCommand(expr_type));
    
    if ( isIntType(expr_type) || isCharType(expr_type) )
    {
        bool is_char_assign = isCharType(expr_type);

        if ( node -> params.empty() )
        {
            code.addConst(0);
            code.add(new AssignCommand(tmp_obj, new NumberArg(0), is_char_assign));
        }
        else
        {
            code.add(new AssignCommand(tmp_obj, getArg(params.front()), is_char_assign));
        }

        _arg = tmp_obj;
        return;
    }
    
    int params_size = 0;

    for ( auto param = params.rbegin(); param != params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
        genParam(*param, info);
        params_size += info.desired_type -> sizeOf();
    }

    code.add(new ParamCommand(tmp_obj, ConversionInfo(nullptr, TypeFactory::getReference(expr_type))));

    params_size += Comp::config().int_size;

    genCall(node -> call_info.callee, params_size);
    _arg = tmp_obj;
}

void GenSSAVisitor::visit(BinaryOperatorNode *node)
{
    auto lhs_type = node -> lhs -> getType();
    auto rhs_type = node -> rhs -> getType();

    if ( isSimpleType(lhs_type.unqualified()) && isSimpleType(rhs_type.unqualified()) )
    {
        auto rhs = getArg(node -> rhs);
        auto lhs = getArg(node -> lhs);

        if ( node -> op_type == BinaryOp::ASSIGN )
            code.add(new AssignCommand(lhs, rhs, lhs_type.unqualified() == BuiltIns::char_type));
        else
            _arg = code.add(new BinaryOpCommand(node -> op_type, lhs, rhs));
    }
    else
    {
        auto lhs_info = node -> call_info.conversions[0];
        auto rhs_info = node -> call_info.conversions[1];

        genParam(node -> rhs, rhs_info);
        genParam(node -> lhs, lhs_info);
        genCall(node -> call_info.callee, lhs_info.desired_type -> sizeOf() + rhs_info.desired_type -> sizeOf());
    }
}

void GenSSAVisitor::visit(StructDeclarationNode *node)
{
    for ( auto decl : node -> inner )
        decl -> accept(*this);
}

void GenSSAVisitor::visit(FunctionDeclarationNode *node)
{
    auto scope_name = node -> definedSymbol -> getScopedTypedName();

    code.newBlock(*node -> definedSymbol -> getScope(), scope_name);
    code.add(new LabelCommand(code.newLabel(scope_name)));

    for ( auto param : node -> params_symbols )
        code.rememberVar(param);

    node -> statements -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode *node)
{
    code.rememberVar(node -> definedSymbol);

    if ( !node -> is_field )
    {
        auto var_type = node -> definedSymbol -> getType();
        auto var_arg = new VariableArg(node -> definedSymbol);

        if ( node -> type_info.is_ref || (node -> type_info.pointer_depth > 0 && !node -> constructor_params.empty()) )
        {
            auto arg = getArg(node -> constructor_params.front());
            code.add(new AssignCommand(var_arg, arg, false));
        }
        else
        {
            if ( isIntType(var_type.unqualified()) || isCharType(var_type.unqualified()) )
            {
                bool is_char_assign = isCharType(var_type.unqualified());
                if ( node -> constructor_params.empty() )
                {
                    code.addConst(0);
                    code.add(new AssignCommand(var_arg, new NumberArg(0), is_char_assign));
                }
                else
                {
                    code.add(new AssignCommand(var_arg, getArg(node -> constructor_params.front()), is_char_assign));
                }
                return;
            }

            if ( node -> inline_info.function_body )
            {
                std::vector<Arg*> args;
                for ( auto param : node -> constructor_params ) {
                    args.push_back(getArg(param));
                }

                genInlineCall(node -> call_info.callee, node -> inline_info, args, var_arg);
                return;
            }

            const auto& params = node -> constructor_params;

            int params_size = 0;

            for ( auto param = params.rbegin(); param != params.rend(); ++param )
            {
                ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
                genParam(*param, info);

                params_size += info.desired_type -> sizeOf();
            }

            code.add(new ParamCommand(var_arg, ConversionInfo(nullptr, TypeFactory::getReference(node -> definedSymbol -> getType().base()))));

            params_size += Comp::config().int_size;
            genCall(node -> call_info.callee, params_size);
        }
    }
}

void GenSSAVisitor::visit(AddrNode *node)
{
    _arg = code.add(new UnaryOpCommand(node -> op, getArg(node -> expr)));
}

void GenSSAVisitor::visit(NullNode *)
{
    code.addConst(0);
    _arg = new NumberArg(0);
}

void GenSSAVisitor::visit(DotNode *node)
{
    if ( node -> member -> getSymbolType() == SymbolType::OVERLOADED_FUNCTION )
    {
        _arg = getArg(node -> base);
    }
    else
    {
        auto var = static_cast<VariableSymbol*>(node -> member);
        auto base_type = static_cast<const StructSymbol*>(node -> base -> getType().unqualified());
        _arg = new DotArg(getArg(node -> base), base_type -> offsetOf(var), var);
    }
}

void GenSSAVisitor::visit(StatementNode *node)
{
    for ( auto stat : node -> statements )
        stat -> accept(*this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
    if ( node -> isTemplateParam() )
    {
        node -> template_num -> accept(*this);
        return;
    }

    if ( node -> variable -> isField() )
    {
        auto this_var = static_cast<VariableSymbol*>(node -> scope -> resolve("this"));
        int offset = static_cast<const StructSymbol*>(this_var -> getType().unqualified()) -> offsetOf(node -> variable);
        _arg = new DotArg(new VariableArg(this_var), offset, node -> variable);
    }
    else
    {
        _arg = new VariableArg(node -> variable);
    }
}

void GenSSAVisitor::visit(StringNode *node)
{
    code.addString(node -> str);
    _arg = new StringArg(node -> str);
}

void GenSSAVisitor::visit(NumberNode *node)
{
    int num = std::stoi(node -> num);

    code.addConst(num);
    _arg = new NumberArg(num);
}

void GenSSAVisitor::visit(CallNode *node)
{
    if ( node -> inline_info.function_body )
    {
        std::vector<Arg*> args;
        for ( auto param : node -> params ) {
            args.push_back(getArg(param));
        }
    
        Arg* this_arg = node -> call_info.callee -> isMethod() ? getArg(node -> caller) : nullptr;

        genInlineCall(node -> call_info.callee, node -> inline_info, args, this_arg);
        return;
    }

    int params_size = 0;
    
    for ( auto param = node -> params.rbegin(); param != node -> params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - node -> params.rbegin()));
        genParam(*param, info);
        params_size += info.desired_type -> sizeOf();
    }

    if ( node -> call_info.callee -> isMethod() )
    {
        params_size += Comp::config().int_size;
        genParam(node -> caller, ConversionInfo(nullptr, TypeFactory::getReference(node -> caller -> getType().base())));
    }

    genCall(node -> call_info.callee, params_size);
}

void GenSSAVisitor::visit(ReturnNode *node)
{
    if ( node -> is_in_inline_call )
    {
        auto expr_type = node -> expr -> getType();
        auto var_arg = new VariableArg(static_cast<VariableSymbol*>(node -> scope -> resolve("$")));

        if ( isSimpleType(expr_type.base()) )
        {
            auto arg = getArg(node -> expr);
            code.add(new AssignCommand(var_arg, arg, isCharType(expr_type.base())));
        }
        else
        {
            code.add(new ParamCommand(getArg(node -> expr), ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base()))));
            code.add(new ParamCommand(var_arg             , ConversionInfo(nullptr, TypeFactory::getReference(expr_type.base()))));
            genCall(static_cast<const StructSymbol*>(expr_type.base()) -> getCopyConstructor(), 2 * Comp::config().int_size);
        }
        _arg = var_arg;
        code.add(new GotoCommand(loop_label.top().second));
        return;
    }

    code.add(new ReturnCommand(getArg(node -> expr), node -> enclosing_func -> type().returnType().isReference()));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *node)
{
    code.rememberVar(node -> definedSymbol);

    auto expr_type = node -> expr -> getType().unqualified();

    auto var = new VariableArg(node -> definedSymbol);

    if ( isIntType(expr_type) || isCharType(expr_type) )
    {
        code.add(new AssignCommand(var, getArg(node -> expr), isCharType(expr_type)));
        return;
    }
    
    genParam(node -> expr, node -> call_info.conversions.front());
    code.add(new ParamCommand(var, ConversionInfo(nullptr, TypeFactory::getReference(expr_type))));
    genCall(node -> call_info.callee, expr_type -> sizeOf() + Comp::config().int_size);
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
    code.add(new GotoCommand(loop_label.top().second));
}

void GenSSAVisitor::visit(ModuleNode* ) { }
void GenSSAVisitor::visit(TypeNode* ) { }
void GenSSAVisitor::visit(FunctionNode* ) { }
void GenSSAVisitor::visit(ModuleMemberAccessNode* ) { }
void GenSSAVisitor::visit(ImportNode *) { }
void GenSSAVisitor::visit(TemplateFunctionNode* ) { }

void GenSSAVisitor::genParam(ExprNode* node, ConversionInfo conversion_info)
{
    code.add(new ParamCommand(getArg(node), conversion_info));
}

void GenSSAVisitor::genCall(const FunctionSymbol* func, int params_size)
{
    _arg = code.add(new CallCommand(func, params_size));
}

void GenSSAVisitor::genInlineCall(const FunctionSymbol* function, const InlineInfo& inline_info, std::vector<Arg*> params, Arg* this_expr)
{
    if ( inline_info.function_body )
    {
        auto exit_from_function_label = code.newLabel();

        loop_label.push({nullptr, exit_from_function_label});

        if ( function -> isMethod() ) {
            params.insert(std::begin(params), this_expr);
        }

        auto param_it = std::begin(params);

        auto return_var = static_cast<VariableSymbol*>(inline_info.function_body -> scope -> resolve("$"));
        code.rememberVar(return_var);

        for ( auto var : inline_info.locals )
        {
            code.rememberVar(var);
                
            auto var_type = var -> getType();
            auto var_arg = new VariableArg(var);

            if ( var_type.isReference() )
            {
                code.add(new AssignRefCommand(var_arg, *param_it));
            }
            else if ( isSimpleType(var_type.base()) )
            {
                code.add(new AssignCommand(var_arg, *param_it, isCharType(var_type.base())));
            }
            else
            {
                code.add(new ParamCommand(*param_it, ConversionInfo(nullptr, TypeFactory::getReference(var_type.base()))));
                code.add(new ParamCommand(var_arg  , ConversionInfo(nullptr, TypeFactory::getReference(var_type.base()))));
                genCall(static_cast<const StructSymbol*>(var_type.base()) -> getCopyConstructor(), 2 * Comp::config().int_size);
            }

            ++param_it;
        }

        inline_info.function_body -> accept(*this);
        code.add(new LabelCommand(exit_from_function_label));

        _arg = new VariableArg(return_var);

        loop_label.pop();
        return;
    }
}

bool GenSSAVisitor::isIntType(const Type* t)    { return t == BuiltIns::int_type; }
bool GenSSAVisitor::isCharType(const Type* t)   { return t == BuiltIns::char_type; }
bool GenSSAVisitor::isPointer(const Type* t)    { return t -> getTypeKind() == TypeKind::POINTER; }
bool GenSSAVisitor::isReference(const Type* t)  { return t -> isReference(); }
bool GenSSAVisitor::isSimpleType(const Type* t) { return isIntType(t) || isCharType(t) || isPointer(t) || isReference(t); }
