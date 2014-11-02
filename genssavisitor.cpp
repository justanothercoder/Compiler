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
#include "optimizer.hpp"
#include "functionsymbol.hpp"
#include "nullnode.hpp"
#include "variabledeclarationnode.hpp"
#include "newexpressionnode.hpp"
#include "asmarraynode.hpp"

#include "typefactory.hpp"
#include "logger.hpp"

GenSSAVisitor::GenSSAVisitor() : _arg(IdType::NOID, -1)
{
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("putchar")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("char")) -> resolve("char")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("int")) -> resolve("int")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;

    code.newBlock(*BuiltIns::global_scope);
}

Arg GenSSAVisitor::getArg(AST *node)
{
    node -> accept(*this);
    return _arg;
}

void GenSSAVisitor::visit(ImportNode *)
{

}

void GenSSAVisitor::visit(IfNode *node)
{
    auto false_label = code.newLabel();
    auto exit_label  = code.newLabel();

    auto expr = getArg(node -> cond);

    code.addConst(0);
    auto zero = Arg(IdType::NUMBER, code.getConstId(0), BuiltIns::int_type);

    auto temp = code.add(Command(SSAOp::EQUALS, expr, zero));

    code.add(Command(SSAOp::IFFALSE, temp, false_label));

    node -> stats_true -> accept(*this);

    code.add(Command(SSAOp::GOTO, exit_label));
    code.add(Command(SSAOp::LABEL, false_label));

    node -> stats_false -> accept(*this);

    code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(ForNode *node)
{
    auto cycle_label = code.newLabel();
    auto exit_label  = code.newLabel();

    node -> init -> accept(*this);
    code.add(Command(SSAOp::LABEL, cycle_label));

    auto cond = getArg(node -> cond);

    code.addConst(0);
    auto zero = Arg(IdType::NUMBER, code.getConstId(0), BuiltIns::int_type);

    auto temp = code.add(Command(SSAOp::EQUALS, cond, zero));

    code.add(Command(SSAOp::IFFALSE, temp, exit_label));

    node -> stats -> accept(*this);
    node -> step  -> accept(*this);

    code.add(Command(SSAOp::GOTO, cycle_label));
    code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(WhileNode *node)
{
    auto exit_label  = code.newLabel();
    auto cycle_label = code.newLabel();

    code.add(Command(SSAOp::LABEL, cycle_label));

    auto cond = getArg(node -> cond);

    code.addConst(0);
    auto zero = Arg(IdType::NUMBER, code.getConstId(0), BuiltIns::int_type);

    auto temp = code.add(Command(SSAOp::EQUALS, cond, zero));

    code.add(Command(SSAOp::IFFALSE, temp, exit_label));

    node -> stats -> accept(*this);

    code.add(Command(SSAOp::GOTO, cycle_label));
    code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(BracketNode *node)
{
    auto expr_info = node -> call_info.conversions.front();
    code.addParamInfo(expr_info);

    code.add(Command(SSAOp::PARAM, 
                     getArg(node -> expr), 
                     Arg(IdType::NOID, code.getInfoId(expr_info))
             )
    );

    auto base_info = ConversionInfo(nullptr, false, false);
    base_info.desired_type = TypeFactory::getReference(node -> base -> getType());
    code.addParamInfo(base_info);

    code.add(Command(SSAOp::PARAM, 
                     getArg(node -> base),
                     Arg(IdType::NOID, code.getInfoId(base_info))
             )
    );

    code.addFunction(node -> call_info.callee);
    _arg = code.add(Command(SSAOp::CALL, 
                            Arg(IdType::PROCEDURE, 
                                code.getFuncId(node -> call_info.callee)), 
                            Arg(IdType::NOID, 
                                node -> expr -> getType() -> getSize() + 
                                GlobalConfig::int_size)
                   )
    );
}

void GenSSAVisitor::visit(UnaryNode *node)
{
    code.add(Command(SSAOp::PARAM, getArg(node -> exp)));

    code.addFunction(node -> call_info.callee);
    _arg = code.add(Command(SSAOp::CALL, 
                            Arg(IdType::PROCEDURE, 
                                code.getFuncId(node -> call_info.callee)), 
                            Arg(IdType::NOID, 
                                node -> exp -> getType() -> getSize())
                    )
    );
}

void GenSSAVisitor::visit(NewExpressionNode *node)
{
    const auto& params = node -> params;

    for ( auto param = params.rbegin(); param != params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM,
                         getArg(*param),
                         Arg(IdType::NOID, code.getInfoId(info))
                        )
                );
    }

//    auto tmp_obj = code.newTemp();
//    tmp_obj.expr_type = node -> getType();

    auto expr_type = node -> getType() -> getUnqualifiedType();

    code.addType(expr_type);

    auto tmp_obj = code.add(Command(SSAOp::NEW, 
                                    Arg(IdType::NOID, 
                                    code.getTypeId(expr_type))
                   ));

    auto info = ConversionInfo(nullptr, false, false);
    info.desired_type = TypeFactory::getReference(expr_type);

    code.addParamInfo(info);
    code.add(Command(SSAOp::PARAM, 
                     tmp_obj,
                     Arg(IdType::NOID, code.getInfoId(info))
                    )
    );

    int params_size = 0;
    for ( auto param : params )
        params_size += param -> getType() -> getSize();    

    params_size += GlobalConfig::int_size;

    code.addFunction(node -> call_info.callee);
    code.add(Command(SSAOp::CALL,
                     Arg(IdType::PROCEDURE,
                         code.getFuncId(node -> call_info.callee)),
                     Arg(IdType::NOID, params_size)
                    )
    );

    _arg = tmp_obj;
}

void GenSSAVisitor::visit(BinaryOperatorNode *node)
{
    auto lhs_type = node -> lhs -> getType();
    auto rhs_type = node -> rhs -> getType();

    if ( lhs_type -> getUnqualifiedType() == BuiltIns::int_type
            && rhs_type -> getUnqualifiedType() == BuiltIns::int_type )
    {
        auto rhs = getArg(node -> rhs);
        auto lhs = getArg(node -> lhs);

        SSAOp op;
        switch ( node -> op_type )
        {
        case BinaryOp::PLUS   : op = SSAOp::PLUS  ; break;
        case BinaryOp::MINUS  : op = SSAOp::MINUS ; break;
        case BinaryOp::MUL    : op = SSAOp::MUL   ; break;
        case BinaryOp::DIV    : op = SSAOp::DIV   ; break;
        case BinaryOp::MOD    : op = SSAOp::MOD   ; break;
        case BinaryOp::ASSIGN : op = SSAOp::ASSIGN; break;
        case BinaryOp::EQUALS : op = SSAOp::EQUALS; break;
        case BinaryOp::NEQUALS: op = SSAOp::NEQUALS; break;        
        default: throw std::logic_error("internal error");
        }

        _arg = code.add(Command(op, lhs, rhs));
    }
    else
    {
        code.add(Command(SSAOp::PARAM, getArg(node -> rhs)));
        code.add(Command(SSAOp::PARAM, getArg(node -> lhs)));

        code.addFunction(node -> call_info.callee);
        _arg = code.add(Command(SSAOp::CALL, 
                                Arg(IdType::PROCEDURE, 
                                    code.getFuncId(node -> call_info.callee)),
                                Arg(IdType::NOID, 
                                    lhs_type -> getSize() + 
                                    rhs_type -> getSize())
                        )
        );
    }
}

void GenSSAVisitor::visit(StructDeclarationNode *node)
{
//    code.newBlock(*node -> definedSymbol);
    for ( auto decl : node -> inner )
        decl -> accept(*this);
//    code.popBlock();
}

void GenSSAVisitor::visit(FunctionDeclarationNode *node)
{
    auto scope_name = node -> definedSymbol -> getScopedTypedName();

    code.newBlock(*node -> definedSymbol, scope_name);
    code.add(Command(SSAOp::LABEL, code.newLabel(scope_name)));

    node -> statements -> accept(*this);

    code.popBlock();
}

void GenSSAVisitor::visit(VariableDeclarationNode *node)
{
    if ( !node -> is_field )
    {
        code.addVariable(node -> definedSymbol);

        if ( node -> type_info.is_ref )
        {
            auto arg = getArg(node -> constructor_call_params.front());
            code.add(Command(SSAOp::ASSIGN,
                             Arg(IdType::VARIABLE,
                                 code.getVarId(node -> definedSymbol),
                                 node -> definedSymbol -> getType()),
                             arg
                            ));
        }
        else if ( node -> type_info.pointer_depth > 0 )
        {
            if ( !node -> constructor_call_params.empty() )
            {
                auto arg = getArg(node -> constructor_call_params.front());
                code.add(Command(SSAOp::ASSIGN,
                                 Arg(IdType::VARIABLE,
                                     code.getVarId(node -> definedSymbol),
                                     node -> definedSymbol -> getType()),
                                 arg
                                ));
            }
        }
        else
        {
            if ( node -> definedSymbol -> getType() -> getUnqualifiedType() == BuiltIns::int_type )
            {
                code.addConst(0);
                code.add(Command(SSAOp::ASSIGN,
                                 Arg(IdType::VARIABLE,
                                     code.getVarId(node -> definedSymbol),
                                     node -> definedSymbol -> getType()),
                                 Arg(IdType::NUMBER, code.getConstId(0), BuiltIns::int_type)
                                ));
                return;
            }

            const auto& params = node -> constructor_call_params;

            for ( auto param = params.rbegin(); param != params.rend(); ++param )
            {
                ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
                code.addParamInfo(info);
                code.add(Command(SSAOp::PARAM,
                                 getArg(*param),
                                 Arg(IdType::NOID, code.getInfoId(info))
                                )
                        );
            }

            auto info = ConversionInfo(nullptr, false, false); //*(node -> call_info.conversions.begin()) 
            info.desired_type = TypeFactory::getReference(node -> definedSymbol -> getType());

            code.addParamInfo(info);

            code.add(Command(SSAOp::PARAM,
                             Arg(IdType::VARIABLE,
                                 code.getVarId(node -> definedSymbol),
                                 node -> definedSymbol -> getType()
                                ),
                             Arg(IdType::NOID, code.getInfoId(info))
                            )
                    );

            int params_size = 0;
            for ( auto param : params )
                params_size += param -> getType() -> getSize();

            params_size += GlobalConfig::int_size;

            code.addFunction(node -> call_info.callee);
            _arg = code.add(Command(SSAOp::CALL,
                                    Arg(IdType::PROCEDURE,
                                        code.getFuncId(node -> call_info.callee)),
                                    Arg(IdType::NOID, params_size))
                           );
        }
    }
}

void GenSSAVisitor::visit(AddrNode *node)
{
    _arg = code.add(Command(SSAOp::ADDR, getArg(node -> expr)));
}

void GenSSAVisitor::visit(NullNode *)
{
    code.addConst(0);
    _arg = Arg(IdType::NUMBER, code.getConstId(0), BuiltIns::int_type);
}

void GenSSAVisitor::visit(DotNode *node)
{
    _arg = code.add(
               Command(SSAOp::DOT,
                       getArg(node -> base),
                       Arg(IdType::VARIABLE, 
                           code.getVarId(node -> member), 
                           node -> member -> getType())
                      )
           );
}

void GenSSAVisitor::visit(StatementNode *node)
{
    for ( auto stat : node -> statements )
        stat -> accept(*this);
}

void GenSSAVisitor::visit(VariableNode *node)
{
    code.addVariable(node -> variable);
    _arg = Arg(IdType::VARIABLE, 
               code.getVarId(node -> variable), 
               node -> variable -> getType()
    );
}

void GenSSAVisitor::visit(StringNode *node)
{
    code.addString(node -> str);
    _arg = Arg(IdType::STRING, code.getStrId(node -> str), BuiltIns::ASCII_string_type);
}

void GenSSAVisitor::visit(NumberNode *node)
{
    int num = std::stoi(node -> num);

    code.addConst(num);
    _arg = Arg(IdType::NUMBER, code.getConstId(num), BuiltIns::int_type);
}

void GenSSAVisitor::visit(CallNode *node)
{
    for ( auto param = node -> params.rbegin(); param != node -> params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - node -> params.rbegin()));
        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM,
                         getArg(*param),
                         Arg(IdType::NOID, code.getInfoId(info))
                        )
                );
    }

    int params_size = 0;
    for ( auto param : node -> params )
        params_size += param -> getType() -> getSize();

    code.addFunction(node -> call_info.callee);
    _arg = code.add(Command(SSAOp::CALL,
                            Arg(IdType::PROCEDURE,
                                code.getFuncId(node -> call_info.callee)),
                            Arg(IdType::NOID, params_size)
                           )
                   );
}

void GenSSAVisitor::visit(ReturnNode *node)
{
    code.add(Command(SSAOp::RETURN, getArg(node -> expr)));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void GenSSAVisitor::visit(AsmArrayNode *node)
{
    auto arr = static_cast<StructSymbol*>(node -> scope);
    int arr_size = boost::get<int>(*node -> scope -> getTemplateInfo().getReplacement("size"));
    
    code.newBlock(*arr);

    code.newBlock(*node -> array_constructor, node -> array_constructor -> getScopedTypedName());
    code.add(Command(SSAOp::LABEL, code.newLabel(node -> array_constructor -> getScopedTypedName())));
    code.popBlock(); 
    
    code.addConst(arr_size);
    code.newBlock(*node -> array_size_func, node -> array_size_func -> getScopedTypedName());
    code.add(Command(SSAOp::LABEL, code.newLabel(node -> array_size_func -> getScopedTypedName())));
    code.add(Command(SSAOp::RETURN, Arg(IdType::NUMBER, code.getConstId(arr_size))));
    code.popBlock();

    code.newBlock(*node -> array_elem_operator, node -> array_elem_operator -> getScopedTypedName());

    auto sym_this = static_cast<VariableSymbol*>(arr -> resolve("~~impl"));
    code.addVariable(sym_this);

    auto param_sym = static_cast<VariableSymbol*>(node -> array_elem_operator -> resolve("__i"));
    code.addVariable(param_sym);

    code.add(Command(SSAOp::LABEL, code.newLabel(node -> array_elem_operator -> getScopedTypedName())));
    code.add(Command(SSAOp::ELEM,  Arg(IdType::VARIABLE, code.getVarId(sym_this)), Arg(IdType::VARIABLE, code.getVarId(param_sym))));
    code.popBlock();

    code.popBlock();
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *)
{

}

void GenSSAVisitor::visit(TemplateStructDeclarationNode *)
{

}

std::string GenSSAVisitor::getString()
{
    return code.toString();
}

const ThreeAddressCode& GenSSAVisitor::getCode() const
{
    return code;
}

void GenSSAVisitor::optimize()
{
    Optimizer optimizer(code);
    optimizer.optimize();
}
