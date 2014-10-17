#include "block.hpp"
#include "scope.hpp"
#include "codeobject.hpp"
#include "globalhelper.hpp"
#include "tempallocator.hpp"
#include "varallocator.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"

Block::Block(Scope& scope, std::string block_name) : scope(scope), block_name(block_name)
{

}

std::string Block::toString()
{
    std::string res = "";

    for ( auto command : code )
        res += commands[command].toString() + "\n";

    return res;
}
    
void Block::genAsm(CodeObject& code_obj) const
{
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
        auto str = GlobalHelper::string_by_id[arg.id];
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
        auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        code_obj.emit("mov qword " + addr + ", " + arg.toString());
        code_obj.emit("lea rax, " + addr);
        return;
    }
    case IdType::TEMP:
    {
//        auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
//        genCommand(commands[arg.id], code_obj);

//        code_obj.emit("mov " + addr + ", rax");
//        code_obj.emit("lea rax, " + addr);

          code_obj.emit("lea rax, [rbp - " + std::to_string(command_offsets[commands[arg.id]]) + "]");

        return;
    }
    case IdType::LABEL:
    {
        code_obj.emit(GlobalHelper::label_name[arg.id] + ":");
        return;
    }
    case IdType::PROCEDURE:
    {
        code_obj.emit("lea rax, " + GlobalHelper::func_by_id[arg.id] -> getScopedTypedName());
        return;
    }
    case IdType::VARIABLE:
    {
        auto variable = GlobalHelper::var_by_id[arg.id];

        if ( variable -> isField() )
        {
			auto _this = scope.resolve("this");

			auto sym = static_cast<VariableSymbol*>(_this);

			auto struc_scope = static_cast<const StructSymbol*>(sym -> getType() -> getSymbol());

			code_obj.emit("mov rax, [rbp - " + std::to_string(scope.getVarAlloc().getAddress(sym)) + "]");
			code_obj.emit("lea rax, [rax - " + std::to_string(struc_scope -> getVarAlloc().getAddress(variable)) + "]");
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
        genArg(command.arg1, code_obj);

        auto base_type = command.arg1.expr_type;
        auto base_sym = base_type -> getUnqualifiedType() -> getSymbol();

        auto member = GlobalHelper::var_by_id[command.arg2.id];

        if ( base_type -> isReference() )
			code_obj.emit("mov rax, [rax]");
		code_obj.emit("lea rax, [rax - " + std::to_string(static_cast<const StructSymbol*>(base_sym) -> getVarAlloc().getAddress(member)) + "]");

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
		auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
		scope.getTempAlloc().claim(GlobalConfig::int_size);

		code_obj.emit("mov " + addr + ", rax");
	   	code_obj.emit("lea rax, " + addr);
        return;
    }
    case SSAOp::PARAM:
    {
        genArg(command.arg1, code_obj);

        const Type *param_type = command.arg1.expr_type;

        if ( param_type -> getUnqualifiedType() == BuiltIns::int_type )
        {
            code_obj.emit("push qword [rax]");            
        }
        else
        {            
            code_obj.emit("push rax");
            code_obj.emit("mov rbx, rsp");
            code_obj.emit("sub rsp, " + std::to_string(param_type -> getSize()));

            code_obj.emit("push rax");
            code_obj.emit("push rbx");
            
            code_obj.emit("call " + static_cast<const StructSymbol*>(param_type) -> getCopyConstructor() -> getScopedTypedName());
            code_obj.emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size + param_type -> getSize()));
        }

        return;
    }
    case SSAOp::CALL:
    {
        auto callee = GlobalHelper::func_by_id[command.arg1.id];

        auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
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
        code_obj.emit(command.arg1.toString());
        return;
    }
    case SSAOp::RETURN:
    {
        return;
    }
    case SSAOp::IF:
    {
        genArg(command.arg1, code_obj);
        code_obj.emit("cmp [rax], qword 0");
        code_obj.emit("jnz " + GlobalHelper::label_name[command.arg2.id]);
        return;        
    }
    case SSAOp::IFFALSE:
    {
        genArg(command.arg1, code_obj);
        code_obj.emit("cmp [rax], qword 0");
        code_obj.emit("jz " + GlobalHelper::label_name[command.arg2.id]);
        return;        
    }
    case SSAOp::GOTO:
    {
        code_obj.emit("jmp " + GlobalHelper::label_name[command.arg2.id]);
        return;
    }
    case SSAOp::ASSIGN:     
    {
        genArg(command.arg2, code_obj);
        code_obj.emit("mov rbx, [rax]");
        
        genArg(command.arg1, code_obj);
        code_obj.emit("mov [rax], rbx");
        return;
    }
    default:
    {
		auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
		scope.getTempAlloc().claim(GlobalConfig::int_size);
        
        genArg(command.arg2, code_obj);
        code_obj.emit("push qword [rax]");

        genArg(command.arg1, code_obj);
        code_obj.emit("pop rbx");
        code_obj.emit("add rbx, [rax]"); //need to add switch for other binary operations
        code_obj.emit("lea rax, " + addr);
        code_obj.emit("mov [rax], rbx"); 
        return;
    }
	}	
}

