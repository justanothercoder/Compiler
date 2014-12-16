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
#include "importnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "externnode.hpp"
#include "typefactory.hpp"
#include "localscope.hpp"

#include "iffalsecommand.hpp"
#include "gotocommand.hpp"
#include "labelcommand.hpp"
#include "elemcommand.hpp"
#include "newcommand.hpp"
#include "assigncommand.hpp"
#include "paramcommand.hpp"
#include "binaryopcommand.hpp"
#include "unaryopcommand.hpp"
#include "dotcommand.hpp"
#include "returncommand.hpp"
#include "callcommand.hpp"

#include "numberarg.hpp"
#include "variablearg.hpp"
#include "stringarg.hpp"

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

void GenSSAVisitor::visit(ExternNode *node)
{
    code.addExternalFunction(node -> definedSymbol);
}

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

    genParam(node -> expr, node -> call_info.conversions.front());

    auto base_info = ConversionInfo(nullptr);
    base_info.desired_type = TypeFactory::getReference(node -> base -> getType().base());
    genParam(node -> base, base_info);

    genCall(node -> call_info.callee, node -> expr -> getType().sizeOf() + GlobalConfig::int_size);
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
    
    if ( expr_type == BuiltIns::int_type || expr_type == BuiltIns::char_type )
    {
        if ( node -> params.empty() )
        {
            code.addConst(0);
            code.add(new AssignCommand(tmp_obj, new NumberArg(0), expr_type != BuiltIns::int_type));
        }
        else
        {
            code.add(new AssignCommand(tmp_obj, getArg(params.front()), expr_type != BuiltIns::int_type));
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

    auto info = ConversionInfo(nullptr);
    info.desired_type = TypeFactory::getReference(expr_type);
    code.add(new ParamCommand(tmp_obj, info));

    params_size += GlobalConfig::int_size;

    genCall(node -> call_info.callee, params_size);
    _arg = tmp_obj;
}

void GenSSAVisitor::visit(BinaryOperatorNode *node)
{
    auto lhs_type = node -> lhs -> getType();
    auto rhs_type = node -> rhs -> getType();

    if ( (lhs_type.unqualified() == BuiltIns::int_type 
       || lhs_type.unqualified() == BuiltIns::char_type
       || lhs_type.unqualified() -> getTypeKind() == TypeKind::POINTER)
      && 
         (rhs_type.unqualified() == BuiltIns::int_type 
       || rhs_type.unqualified() == BuiltIns::char_type
       || rhs_type.unqualified() -> getTypeKind() == TypeKind::POINTER) )
    {
        auto rhs = getArg(node -> rhs);
        auto lhs = getArg(node -> lhs);

        if ( node -> op_type == BinaryOp::ASSIGN )
            code.add(new AssignCommand(lhs, rhs, lhs_type.unqualified() != BuiltIns::int_type));
        else
            _arg = code.add(new BinaryOpCommand(node -> op_type, lhs, rhs));
    }
    else
    {
        ConversionInfo lhs_info(nullptr);
        ConversionInfo rhs_info(nullptr);

        if ( node -> call_info.callee -> isMethod() )
        {
            rhs_info = node -> call_info.conversions.front();

            lhs_info = ConversionInfo(nullptr);
            lhs_info.desired_type = TypeFactory::getReference(node -> lhs -> getType().unqualified());
        }
        else
        {   
            rhs_info = node -> call_info.conversions[1];
            lhs_info = node -> call_info.conversions[0];
        }
        
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

    node -> statements -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode *node)
{
    if ( !node -> is_field )
    {
        auto var_type = node -> definedSymbol -> getType();
        auto var_arg = new VariableArg(node -> definedSymbol);

        if ( node -> type_info.is_ref )
        {
            auto arg = getArg(node -> constructor_call_params.front());
            code.add(new AssignCommand(var_arg, arg, false));
        }
        else if ( node -> type_info.pointer_depth > 0 )
        {
            if ( !node -> constructor_call_params.empty() )
            {
                auto arg = getArg(node -> constructor_call_params.front());
                code.add(new AssignCommand(var_arg, arg, false));
            }
        }
        else
        {
            if ( var_type.unqualified() == BuiltIns::int_type
              || var_type.unqualified() == BuiltIns::char_type )
            {
                bool is_char_assign = (var_type.unqualified() != BuiltIns::int_type);
                if ( node -> constructor_call_params.empty() )
                {
                    code.addConst(0);
                    code.add(new AssignCommand(var_arg, new NumberArg(0), is_char_assign));
                }
                else
                {
                    code.add(new AssignCommand(var_arg, getArg(node -> constructor_call_params.front()), is_char_assign));
                }
                return;
            }

            const auto& params = node -> constructor_call_params;

            int params_size = 0;

            for ( auto param = params.rbegin(); param != params.rend(); ++param )
            {
                ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
                genParam(*param, info);

                params_size += info.desired_type -> sizeOf();
            }

            auto info = ConversionInfo(nullptr); 
            info.desired_type = TypeFactory::getReference(node -> definedSymbol -> getType().base());
            code.add(new ParamCommand(var_arg, info));

            params_size += GlobalConfig::int_size;
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
        _arg = code.add(new DotCommand(getArg(node -> base), base_type -> offsetOf(var), var));
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
    
    _arg = new VariableArg(node -> variable);
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
    if ( shouldBeInlined(node -> call_info) )
    {
        genInlineCall(node);
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
        params_size += GlobalConfig::int_size;
        auto info = ConversionInfo(nullptr);
        info.desired_type = TypeFactory::getReference(node -> caller -> getType().base()); 
        genParam(node -> caller, info);
    }

    genCall(node -> call_info.callee, params_size);
}

void GenSSAVisitor::visit(ReturnNode *node)
{
    code.add(new ReturnCommand(getArg(node -> expr), node -> enclosing_func -> type().returnType().isReference()));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto expr_type = node -> expr -> getType().unqualified();

    auto var = new VariableArg(node -> definedSymbol);

    if ( expr_type == BuiltIns::int_type || expr_type == BuiltIns::char_type )
    {
        code.add(new AssignCommand(var, getArg(node -> expr), expr_type != BuiltIns::int_type));
        return;
    }
    
    auto info = *std::begin(node -> call_info.conversions);
    genParam(node -> expr, info);

    info = ConversionInfo(nullptr);
    info.desired_type = TypeFactory::getReference(expr_type);
    code.add(new ParamCommand(var, info));

    genCall(node -> call_info.callee, expr_type -> sizeOf() + GlobalConfig::int_size);
}

void GenSSAVisitor::visit(TemplateStructDeclarationNode *node)
{
    for ( auto instance : node -> instances )
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

bool GenSSAVisitor::shouldBeInlined(CallInfo call_info)
{
    return false;

    auto func = call_info.callee;

    return std::all_of(std::begin(func -> type().typeInfo().params_types)
                     , std::end(func -> type().typeInfo().params_types)
                     , [](VariableType type) {
                         return type.base() == BuiltIns::int_type 
                             || type.base() == BuiltIns::char_type 
                             || type.isReference() 
                             || type.base() -> getTypeKind() == TypeKind::POINTER;
    });
}

void GenSSAVisitor::genInlineCall(CallNode* )
{
/*
    const auto& call_info = node -> call_info;
    const auto& params    = node -> params;

    auto decl = call_info.callee -> function_decl;
    auto tree = decl -> getChildren().front() -> copyTree();

    tree -> scope = new LocalScope(node -> scope);
    
    auto it = std::begin(params);

    for ( auto param : decl -> params )
    {
        auto param_type = fromTypeInfo(param.second, tree -> scope);
        auto param_symbol = new VariableSymbol(param.first, param_type);
        tree -> scope -> define(param_symbol);

        ConversionInfo info = *(call_info.conversions.rbegin() + (it - std::begin(params)));
        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM, getArg(*it), code.getInfoId(info)));
            
        code.addVariable(param_symbol);
        Arg var(IdType::VARIABLE, code.getVarId(param_symbol), param_symbol -> getType().base()); 

        ConversionInfo copy_info(nullptr);
        copy_info.desired_type = TypeFactory::getReference(param_type.unqualified());
        code.addParamInfo(copy_info);
        code.add(Command(SSAOp::PARAM, var, code.getInfoId(copy_info)));

        auto copy_constr = static_cast<const StructSymbol*>(param_type.unqualified()) -> getCopyConstructor();
        code.addFunction(copy_constr);

        _arg = code.add(Command(SSAOp::CALL, 
                                Arg(IdType::PROCEDURE, 
                                    code.getFuncId(copy_constr)), 
                                Arg(IdType::NOID, 
                                    param_type.sizeOf() + 
                                    GlobalConfig::int_size)
                       )
    }
    
    tree -> accept(*this);
*/    
}
    
void GenSSAVisitor::genParam(ExprNode* node, ConversionInfo conversion_info)
{
    code.add(new ParamCommand(getArg(node), conversion_info));
}

void GenSSAVisitor::genCall(const FunctionSymbol* func, int params_size)
{
    _arg = code.add(new CallCommand(func, params_size));
}
