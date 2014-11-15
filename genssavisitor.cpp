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
#include "templatestructdeclarationnode.hpp"
#include "importnode.hpp"
#include "varinfertypedeclarationnode.hpp"

#include "typefactory.hpp"
#include "logger.hpp"

GenSSAVisitor::GenSSAVisitor() : _arg(IdType::NOID, -1)
{
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("putchar")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("__fopen")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("__fread")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("__fwrite")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("__fclose")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(BuiltIns::global_scope -> resolve("print")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("string")) -> resolve("operator[]")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("string")) -> resolve("length")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("string")) -> resolve("operator+")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("string")) -> resolve("string")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    for ( auto func : dynamic_cast<const OverloadedFunctionSymbol*>(dynamic_cast<VariableSymbol*>(dynamic_cast<StructSymbol*>(BuiltIns::global_scope -> resolve("string")) -> resolve("operator=")) -> getType()) -> getTypeInfo().symbols )
        code.globaltable.has_definition[dynamic_cast<FunctionSymbol*>(func.second)] = false;
    
    code.newBlock(*BuiltIns::global_scope);
}

Arg GenSSAVisitor::getArg(AST *node)
{
    node -> accept(*this);
    return _arg;
}

void GenSSAVisitor::visit(ImportNode *node)
{
    node -> root -> accept(*this);
}

void GenSSAVisitor::visit(IfNode *node)
{
    auto false_label = code.newLabel();
    auto exit_label  = code.newLabel();

    auto expr = getArg(node -> cond);

    code.add(Command(SSAOp::IFFALSE, expr, false_label));

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

    code.add(Command(SSAOp::IFFALSE, cond, exit_label));

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

    code.add(Command(SSAOp::IFFALSE, cond, exit_label));

    node -> stats -> accept(*this);

    code.add(Command(SSAOp::GOTO, cycle_label));
    code.add(Command(SSAOp::LABEL, exit_label));
}

void GenSSAVisitor::visit(BracketNode *node)
{
    if ( node -> base -> getType() -> getTypeKind() == TypeKind::ARRAY )
    {
        _arg = code.add(Command(SSAOp::ELEM,
                                getArg(node -> base),
                                getArg(node -> expr)
                        )
        );
        return;
    }

    auto expr_info = node -> call_info.conversions.front();
    code.addParamInfo(expr_info);

    code.add(Command(SSAOp::PARAM, 
                     getArg(node -> expr), 
                     Arg(IdType::NOID, code.getInfoId(expr_info))
             )
    );

    auto base_info = ConversionInfo(nullptr);
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

    auto expr_type = node -> getType() -> getUnqualifiedType();

    code.addType(expr_type);

    auto tmp_obj = code.add(Command(SSAOp::NEW, 
                                    Arg(IdType::NOID, 
                                        code.getTypeId(expr_type))
                   )
    );
    
    if ( expr_type == BuiltIns::int_type || expr_type == BuiltIns::char_type )
    {
        SSAOp assign_op = expr_type == BuiltIns::int_type ? SSAOp::ASSIGN : SSAOp::ASSIGNCHAR;

        if ( node -> params.empty() )
        {
            code.addConst(0);
            code.add(Command(assign_op,
                             tmp_obj,
                             Arg(IdType::NUMBER, 
                                 code.getConstId(0), 
                                 BuiltIns::int_type)
                            )
            );
        }
        else
        {
            code.add(Command(assign_op,
                             tmp_obj,
                             getArg(*std::begin(params))
                            )
            );
        }

        _arg = tmp_obj;
        return;
    }
    
    int params_size = 0;

    for ( auto param = params.rbegin(); param != params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM,
                         getArg(*param),
                         Arg(IdType::NOID, code.getInfoId(info))
                        )
                );

        params_size += info.desired_type -> getSize();
    }

    

    auto info = ConversionInfo(nullptr);
    info.desired_type = TypeFactory::getReference(expr_type);

    code.addParamInfo(info);
    code.add(Command(SSAOp::PARAM, 
                     tmp_obj,
                     Arg(IdType::NOID, code.getInfoId(info))
                    )
    );

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

    if ( (lhs_type -> getUnqualifiedType() == BuiltIns::int_type || lhs_type -> getUnqualifiedType() == BuiltIns::char_type)
      && (rhs_type -> getUnqualifiedType() == BuiltIns::int_type || rhs_type -> getUnqualifiedType() == BuiltIns::char_type) )
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
        case BinaryOp::ASSIGN : op = (lhs_type -> getUnqualifiedType() == BuiltIns::int_type ? SSAOp::ASSIGN : SSAOp::ASSIGNCHAR); break;
        case BinaryOp::EQUALS : op = SSAOp::EQUALS; break;
        case BinaryOp::NEQUALS: op = SSAOp::NEQUALS; break;        
        default: throw std::logic_error("internal error");
        }

        _arg = code.add(Command(op, lhs, rhs));
    }
    else
    {
        if ( node -> call_info.callee -> isMethod() )
        {
            auto rhs_info = *std::begin(node -> call_info.conversions);
            code.addParamInfo(rhs_info);
            code.add(Command(SSAOp::PARAM, getArg(node -> rhs), Arg(IdType::NOID, code.getInfoId(rhs_info))));

            auto lhs_info = ConversionInfo(nullptr);
            lhs_info.desired_type = TypeFactory::getReference(node -> lhs -> getType() -> getUnqualifiedType());

            code.addParamInfo(lhs_info);                      
            code.add(Command(SSAOp::PARAM, getArg(node -> lhs), Arg(IdType::NOID, code.getInfoId(lhs_info))));

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
        else
        {       
            auto rhs_info = *(std::begin(node -> call_info.conversions) + 1);
            auto lhs_info = *(std::begin(node -> call_info.conversions) + 0);

            code.addParamInfo(rhs_info);
            code.addParamInfo(lhs_info);

            code.add(Command(SSAOp::PARAM, getArg(node -> rhs), Arg(IdType::NOID, code.getInfoId(rhs_info))));
            code.add(Command(SSAOp::PARAM, getArg(node -> lhs), Arg(IdType::NOID, code.getInfoId(lhs_info))));

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
}

void GenSSAVisitor::visit(StructDeclarationNode *node)
{
    for ( auto decl : node -> inner )
        decl -> accept(*this);
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

        auto var_type = node -> definedSymbol -> getType();

        if ( node -> type_info.is_ref )
        {
            auto arg = getArg(node -> constructor_call_params.front());
            code.add(Command(SSAOp::ASSIGN,
                             Arg(IdType::VARIABLE,
                                 code.getVarId(node -> definedSymbol),
                                 var_type),
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
                                     var_type),
                                 arg
                                ));
            }
        }
        else
        {
            if ( var_type -> getUnqualifiedType() == BuiltIns::int_type
              || var_type -> getUnqualifiedType() == BuiltIns::char_type )
            {
                SSAOp assign_op = (var_type -> getUnqualifiedType() == BuiltIns::int_type ? SSAOp::ASSIGN : SSAOp::ASSIGNCHAR);
                if ( node -> constructor_call_params.empty() )
                {
                    code.addConst(0);
                    code.add(Command(assign_op,
                                     Arg(IdType::VARIABLE,
                                         code.getVarId(node -> definedSymbol),
                                         var_type),
                                     Arg(IdType::NUMBER, 
                                         code.getConstId(0), 
                                         BuiltIns::int_type)
                                    ));
                }
                else
                {
                    code.add(Command(assign_op,
                                     Arg(IdType::VARIABLE,
                                         code.getVarId(node -> definedSymbol),
                                         var_type),
                                     getArg(*std::begin(node -> constructor_call_params))
                                    ));
                }
                return;
            }

            const auto& params = node -> constructor_call_params;

            int params_size = 0;

            for ( auto param = params.rbegin(); param != params.rend(); ++param )
            {
                ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - params.rbegin()));
                code.addParamInfo(info);
                code.add(Command(SSAOp::PARAM,
                                 getArg(*param),
                                 Arg(IdType::NOID, code.getInfoId(info))
                                )
                        );

                params_size += info.desired_type -> getSize();
            }

            auto info = ConversionInfo(nullptr); 
            info.desired_type = TypeFactory::getReference(node -> definedSymbol -> getType());

            code.addParamInfo(info);

            code.add(Command(SSAOp::PARAM,
                             Arg(IdType::VARIABLE,
                                 code.getVarId(node -> definedSymbol),
                                 var_type
                                ),
                             Arg(IdType::NOID, code.getInfoId(info))
                            )
                    );


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
    if ( node -> member -> getType() -> getTypeKind() == TypeKind::OVERLOADEDFUNCTION )
    {
        _arg = getArg(node -> base);
    }
    else
    {
        code.addVariable(node -> member);
        _arg = code.add(
                   Command(SSAOp::DOT,
                           getArg(node -> base),
                           Arg(IdType::VARIABLE, 
                               code.getVarId(node -> member), 
                               node -> member -> getType())
                          )
               );
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
    int params_size = 0;
    
    for ( auto param = node -> params.rbegin(); param != node -> params.rend(); ++param )
    {
        ConversionInfo info = *(node -> call_info.conversions.rbegin() + (param - node -> params.rbegin()));
        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM,
                         getArg(*param),
                         Arg(IdType::NOID, code.getInfoId(info))
                        )
                );

        params_size += info.desired_type -> getSize();
    }

    if ( node -> call_info.callee -> isMethod() )
    {
        params_size += GlobalConfig::int_size;
        
        auto info = ConversionInfo(nullptr);
        info.desired_type = TypeFactory::getReference(node -> caller -> getType()); 

        code.addParamInfo(info);
        code.add(Command(SSAOp::PARAM,
                         getArg(node -> caller),
                         Arg(IdType::NOID, code.getInfoId(info))
                        )
                );
    }

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
    if ( node -> enclosing_func -> return_type -> isReference() )
        code.add(Command(SSAOp::RETURNREF, getArg(node -> expr)));
    else
        code.add(Command(SSAOp::RETURN, getArg(node -> expr)));
}

void GenSSAVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void GenSSAVisitor::visit(VarInferTypeDeclarationNode *node)
{
    auto expr_type = node -> expr -> getType() -> getUnqualifiedType();

    code.addVariable(node -> definedSymbol);
    auto var = Arg(IdType::VARIABLE, code.getVarId(node -> definedSymbol), expr_type);

    if ( expr_type == BuiltIns::int_type || expr_type == BuiltIns::char_type )
    {
        code.add(Command(expr_type == BuiltIns::int_type ? SSAOp::ASSIGN : SSAOp::ASSIGNCHAR, var, getArg(node -> expr)));
        return;
    }
    
    auto info = *std::begin(node -> call_info.conversions);
    code.addParamInfo(info);
    code.add(Command(SSAOp::PARAM,
                     getArg(node -> expr),
                     Arg(IdType::NOID, code.getInfoId(info))
                    )
            );

    info = ConversionInfo(nullptr);
    info.desired_type = TypeFactory::getReference(expr_type);

    code.addParamInfo(info);
    code.add(Command(SSAOp::PARAM, 
                     var,
                     Arg(IdType::NOID, code.getInfoId(info))
                    )
    );

    int params_size = expr_type -> getSize() + GlobalConfig::int_size;

    code.addFunction(node -> call_info.callee);
    code.add(Command(SSAOp::CALL,
                     Arg(IdType::PROCEDURE,
                         code.getFuncId(node -> call_info.callee)),
                     Arg(IdType::NOID, params_size)
                    )
    );
}

void GenSSAVisitor::visit(TemplateStructDeclarationNode *node)
{
    for ( auto instance : node -> instances )
        instance -> accept(*this);
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
