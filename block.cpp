#include "block.hpp"
#include "scope.hpp"
#include "codeobject.hpp"
#include "tempallocator.hpp"
#include "varallocator.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"
#include "referencetype.hpp"

#include <sstream>
#include "logger.hpp"

Block::Block(Scope& scope, GlobalTable& table, std::string block_name) : scope(scope), block_name(block_name), table(table)
{

}

std::string Block::toString()
{
    std::string res = "";

    for ( auto command : code )
        res += toString(commands[command]) + "\n";

    return res;
}

void Block::genAsm(CodeObject& code_obj) const
{
    if ( code.empty() )
        return;

    auto it = std::begin(code);

    if ( commands[*it].op == SSAOp::LABEL )
    {
        genCommand(commands[*it], code_obj);
        ++it;
    }

    code_obj.emit("push rbp");
    code_obj.emit("mov rbp, rsp");

    if ( scope.getVarAlloc().getSpace() > 0 )
        code_obj.emit("sub rsp, " + std::to_string(scope.getVarAlloc().getSpace()));
    code_obj.emit("sub rsp, " + std::to_string(scope.getTempAlloc().getSpaceNeeded()));

    for ( ; it != std::end(code); ++it )
        genCommand(commands[*it], code_obj);

    code_obj.emit("mov rsp, rbp");
    code_obj.emit("pop rbp");
}

void Block::genArg(Arg arg, CodeObject& code_obj) const
{
    switch ( arg.type )
    {
    case IdType::NOID  : return;
    case IdType::STRING:
    {
        auto str_label = "string_label" + std::to_string(arg.id);

        code_obj.emit("section .data");

        std::string res = "0";
        auto str = table.string_by_id[arg.id];
        for ( int i = str.length() - 1; i >= 0; --i )
        {
            if ( i >= 1 && str[i - 1] == '\\' && str[i] == 'n' )
            {
                res.append(", 10");
                --i;
            }
            else
            {
                res.append(", " + std::to_string(static_cast<int>(str[i])));
            }
        }

        code_obj.emit("@" + str_label + ": db " + res);
        code_obj.emit(str_label + ": equ $ - 1");
        code_obj.emit("section .text");
        code_obj.emit("lea rax, [" + str_label + "]");

        return;
    }
    case IdType::NUMBER:
    {
        auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(GlobalConfig::int_size);

        code_obj.emit("mov qword " + addr + ", " + toString(arg));
        code_obj.emit("lea rax, " + addr);
        return;
    }
    case IdType::TEMP:
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(command_offsets.at(commands[arg.id])) + "]");
        return;
    }
    case IdType::LABEL:
    {
        code_obj.emit(table.label_name.at(arg.id) + ":");
        return;
    }
    case IdType::PROCEDURE:
    {
        code_obj.emit("lea rax, " + table.func_by_id[arg.id] -> getScopedTypedName());
        return;
    }
    case IdType::VARIABLE:
    {
        auto variable = table.var_by_id[arg.id];

        if ( variable -> isField() )
        {
            auto _this = scope.resolve("this");

            auto sym = static_cast<VariableSymbol*>(_this);

            auto struc_scope = static_cast<const StructSymbol*>(sym -> getType() -> getSymbol());

            code_obj.emit("mov rax, [rbp - " + std::to_string(scope.getVarAlloc().getAddress(sym)) + "]");
            code_obj.emit("lea rax, [rax - " + std::to_string(struc_scope -> getVarAlloc().getAddress(variable)) + "]");
        }
        else if ( variable -> getType() -> isReference() )
        {
            code_obj.emit("mov rax, [rbp - " + std::to_string(scope.getVarAlloc().getAddress(variable)) + "]");
        }
        else
        {
            code_obj.emit("lea rax, [rbp - " + std::to_string(scope.getVarAlloc().getAddress(variable)) + "]");
        }
        return;
    }
    }
}

void Block::genCommand(Command command, CodeObject& code_obj) const
{
    switch ( command.op )
    {
    case SSAOp::DOT:
    {

//        genArg(command.arg1, code_obj);

        auto base_type = command.arg1.expr_type;
        auto base_sym = base_type -> getUnqualifiedType() -> getSymbol();

        auto member = table.var_by_id[command.arg2.id];
        /*
                if ( base_type -> isReference() )
                    code_obj.emit("mov rax, [rax]");
                code_obj.emit("lea rax, [rax - " + std::to_string(static_cast<const StructSymbol*>(base_sym) -> getVarAlloc().getAddress(member)) + "]");
        */
        int arg_addr;
        if ( command.arg1.type == IdType::VARIABLE )
        {
            if ( base_type -> isReference() )
            {
                genArg(command.arg1, code_obj);

                auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
                command_offsets[command] = GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset());
                scope.getTempAlloc().claim(GlobalConfig::int_size);

                code_obj.emit("mov rax, [rax - " + std::to_string(static_cast<const StructSymbol*>(base_sym) -> getVarAlloc().getAddress(member)) + "]");
                code_obj.emit("mov " + addr + ", rax");
//                code_obj.emit("lea rax, [rax - " + std::to_string(static_cast<const StructSymbol*>(base_sym) -> getVarAlloc().getAddress(member)) + "]");

                return;
            }

            arg_addr = scope.getVarAlloc().getAddress(table.var_by_id[command.arg1.id]);
        }
        else if ( command.arg1.type == IdType::TEMP )
        {
            arg_addr = command_offsets[commands[command.arg1.id]];
        }
        else
        {
            arg_addr = 0;
        }

        command_offsets[command] = arg_addr + static_cast<const StructSymbol*>(base_sym) -> getVarAlloc().getAddress(member);

        return;
    }
    case SSAOp::DEREF:
    {
        genArg(command.arg1, code_obj);

        //if ( expr -> getType() -> isReference() )
        //  code_obj.emit("mov rax, [rax]");

        code_obj.emit("mov rax, [rax]");
        return;
    }
    case SSAOp::ADDR:
    {
        command_offsets[command] = scope.getTempAlloc().getOffset();
        auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(GlobalConfig::int_size);

        code_obj.emit("mov " + addr + ", rax");
        code_obj.emit("lea rax, " + addr);
        return;
    }
    case SSAOp::PARAM:
    {
        genArg(command.arg1, code_obj);

        const auto& conversion_info = table.info_by_id.at(command.arg2.id);

        const Type *param_type = command.arg1.expr_type;
           
        if ( param_type -> getUnqualifiedType() == BuiltIns::int_type )
        {
            code_obj.emit("push qword [rax]");
        }
        else if ( conversion_info.desired_type -> isReference() )
        {
            code_obj.emit("push rax");
        }
        else if ( conversion_info.conversion )
        {
            auto conv = conversion_info.conversion;

            code_obj.emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
            code_obj.emit("sub rsp, " + std::to_string(param_type -> getSize()));

            code_obj.emit("push rax");
            code_obj.emit("push rbx");

            code_obj.emit("call " + conv -> getScopedTypedName());
            code_obj.emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size));
        }
        else
        {
            code_obj.emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
            code_obj.emit("sub rsp, " + std::to_string(param_type -> getSize()));

            if ( param_type -> isReference() )
            {
                code_obj.emit("mov rax, [rax]");
                param_type = static_cast<const ReferenceType*>(param_type) -> type;
            }

            code_obj.emit("push rax");
            code_obj.emit("push rbx");

            code_obj.emit("call " + dynamic_cast<const StructSymbol*>(param_type) -> getCopyConstructor() -> getScopedTypedName());
            code_obj.emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size));
        }

        return;
    }
    case SSAOp::CALL:
    {
        auto callee = table.func_by_id[command.arg1.id];

        command_offsets[command] = GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset());

        auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(callee -> return_type -> getSize());

        code_obj.emit("lea rax, " + addr);
        code_obj.emit("push rax");

        code_obj.emit("call " + callee -> getScopedTypedName());
        code_obj.emit("add rsp, " +  std::to_string(command.arg2.id));

        code_obj.emit("pop rax");
        /*
                if ( callee -> isMethod() )
                    code_obj.genCallCode(call_info, params, caller -> gen(), caller -> getType() -> isReference());
                else
                {
                    CodeObject empty;
                    code_obj.genCallCode(call_info, params, empty, false);
                }

                code_obj.emit("pop rax");
        */
        return;
    }
    case SSAOp::LABEL:
    {
        code_obj.emit(toString(command.arg1));
        return;
    }
    case SSAOp::RETURN:
    {
        return;
    }
    case SSAOp::IF:
    {
        genArg(command.arg1, code_obj);
        code_obj.emit("cmp qword [rax], qword 0");
        code_obj.emit("jnz " + table.label_name.at(command.arg2.id));
        return;
    }
    case SSAOp::IFFALSE:
    {
        genArg(command.arg1, code_obj);
        code_obj.emit("cmp qword [rax], qword 0");
        code_obj.emit("jz " + table.label_name.at(command.arg2.id));
        return;
    }
    case SSAOp::GOTO:
    {
        code_obj.emit("jmp " + table.label_name.at(command.arg1.id));
        return;
    }
    case SSAOp::ASSIGN:
    {
        if ( command.arg1.expr_type -> getUnqualifiedType() == BuiltIns::int_type
                && command.arg2.expr_type -> getUnqualifiedType() == BuiltIns::int_type )
        {
            genArg(command.arg2, code_obj);
            code_obj.emit("mov rbx, [rax]");

            genArg(command.arg1, code_obj);
            code_obj.emit("mov [rax], rbx");
            return;
        }
    }
    case SSAOp::NEW:
    {
        auto object_type = table.type_by_id.at(command.arg1.id);

        command_offsets[command] = GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset());

        auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(object_type -> getSize());

        auto addr2 = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(GlobalConfig::int_size);

        return;
/*
        CodeObject new_place;
        new_place.emit("lea rax, " + addr);

        code_obj.genCallCode(call_info, params, new_place, false);

        code_obj.emit("lea rax, " + addr);
        code_obj.emit("mov " + addr2 + ", rax");
        code_obj.emit("lea rax, " + addr2);

        return code_obj;
*/       
    }    
    case SSAOp::PLUS: case SSAOp::NEQUALS: case SSAOp::EQUALS:
    case SSAOp::MINUS: case SSAOp::MUL: case SSAOp::DIV:
    case SSAOp::MOD:
    {
        command_offsets[command] = GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset());

        auto addr = "[rbp - " + std::to_string(GlobalTable::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        scope.getTempAlloc().claim(GlobalConfig::int_size);

        genArg(command.arg2, code_obj);
        code_obj.emit("push qword [rax]");

        genArg(command.arg1, code_obj);
        code_obj.emit("pop rbx");
        switch ( command.op )
        {
            case SSAOp::PLUS   : code_obj.emit("add rbx, [rax]"); break;
            case SSAOp::MINUS  : code_obj.emit("sub rbx, [rax]"); break;
            case SSAOp::MUL    : code_obj.emit("imul rbx, [rax]"); break;
            case SSAOp::DIV    : code_obj.emit("xor rdx, rdx"); code_obj.emit("idiv rbx, [rax]"); break;
            case SSAOp::MOD    : code_obj.emit("xor rdx, rdx"); code_obj.emit("idiv rbx, [rax]"); code_obj.emit("mov rbx, rdx"); break;
            case SSAOp::EQUALS : code_obj.emit("cmp rbx, [rax]"); code_obj.emit("mov rbx, qword 0"); code_obj.emit("sete bl"); break;
            case SSAOp::NEQUALS: code_obj.emit("cmp rbx, [rax]"); code_obj.emit("mov rbx, qword 0"); code_obj.emit("setne bl"); break;
            default:
                   throw std::logic_error("internal error.");
        }
                                
        code_obj.emit("lea rax, " + addr);
        code_obj.emit("mov [rax], rbx");
        return;
    }
    case SSAOp::ELEM:
    {

        return;
    }
    default:
    {
        Logger::log(toString(command));
        throw "internal error.";
    }
    }
}

std::string Block::toString(Command command) const
{
    switch ( command.op )
    {
    case SSAOp::ASSIGN : return toString(command.arg1) + " = "  + toString(command.arg2);
    case SSAOp::PLUS   : return toString(command.arg1) + " + "  + toString(command.arg2);
    case SSAOp::MINUS  : return toString(command.arg1) + " - "  + toString(command.arg2);
    case SSAOp::MUL    : return toString(command.arg1) + " * "  + toString(command.arg2);
    case SSAOp::DIV    : return toString(command.arg1) + " / "  + toString(command.arg2);
    case SSAOp::MOD    : return toString(command.arg1) + " % "  + toString(command.arg2);
    case SSAOp::EQUALS : return toString(command.arg1) + " == " + toString(command.arg2);
    case SSAOp::NEQUALS: return toString(command.arg1) + " != " + toString(command.arg2);
    case SSAOp::ELEM   : return toString(command.arg1) + "[" + toString(command.arg2) + "]";
    case SSAOp::DOT    : return toString(command.arg1) + "." + toString(command.arg2);
    case SSAOp::DEREF  : return "*" + toString(command.arg1);
    case SSAOp::ADDR   : return "&" + toString(command.arg1);
    case SSAOp::PARAM  : return "param " + toString(command.arg1);
    case SSAOp::CALL   : return "call " + toString(command.arg1) + ' ' + std::to_string(command.arg2.id);
    case SSAOp::LABEL  : return toString(command.arg1);
    case SSAOp::RETURN : return "return " + toString(command.arg1);
    case SSAOp::IF     : return "if " + toString(command.arg1) + " goto " + toString(command.arg2);
    case SSAOp::IFFALSE: return "ifFalse " + toString(command.arg1) + " goto " + toString(command.arg2);
    case SSAOp::GOTO   : return "goto " + toString(command.arg1);
    case SSAOp::NEW    : return "new " + table.type_by_id[command.arg1.id] -> getName();
    default:
        throw std::logic_error("not all SSAOp catched in Block::toString");
    }
}

std::string Block::toString(Arg arg) const
{
    switch ( arg.type )
    {
    case IdType::NOID     : return "noid";
    case IdType::STRING   : return '"' + table.string_by_id[arg.id] + '"';
    case IdType::NUMBER   : return std::to_string(table.id_to_num[arg.id]);
    case IdType::TEMP     : return "temp_" + std::to_string(arg.id);
    case IdType::LABEL    : return table.label_name[arg.id] + ":";
    case IdType::PROCEDURE: return table.func_by_id[arg.id] -> getScopedTypedName();
    case IdType::VARIABLE : return table.var_by_id[arg.id] -> getName();
    }
}

