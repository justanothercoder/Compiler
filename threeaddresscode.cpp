#include "threeaddresscode.hpp"
#include "variablesymbol.hpp"
#include "structsymbol.hpp"

Block::Block(Scope& scope) : scope(scope)
{

}

std::string Block::toString()
{
    std::string res = "";

    for ( auto command : code )
        res += commands[command].toString() + "\n";

    return res;
}
    
void Block::genAsm(CodeObject& code_obj)
{
    for ( auto command_id : code )
        genCommand(commands[command_id], code_obj);
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
        auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
        genCommand(commands[arg.id], code_obj);

        code_obj.emit("mov " + addr + ", rax");
        code_obj.emit("lea rax, " + addr);

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
		auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(&scope, scope.getTempAlloc().getOffset())) + "]";
		scope.getTempAlloc().claim(GlobalConfig::int_size);

		code_obj.emit("mov " + addr + ", rax");
	   	code_obj.emit("lea rax, " + addr);
        return;
    }
    case SSAOp::PARAM:
    {
        return;
    }
    case SSAOp::CALL:
    {
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
        code_obj.emit("push [rax]");

        genArg(command.arg1, code_obj);
        code_obj.emit("pop rbx");
        code_obj.emit("add rbx, [rax]"); //need to add switch for other binary operations
        code_obj.emit("lea rax, " + addr);
        code_obj.emit("mov rax, [rbx"); 
        return;
    }
	}	
}

Arg ThreeAddressCode::newTemp()
{
	static int temp_num = 0;
	return Arg(IdType::TEMP, ++temp_num);
}
	
Arg ThreeAddressCode::newLabel(std::string label)
{
	static int label_num = 0;

	++label_num;

	if ( label == "" )
		GlobalHelper::label_name[label_num] = "label_" + std::to_string(label_num);
	else
		GlobalHelper::label_name[label_num] = label;

	return Arg(IdType::LABEL, label_num);
}
	
Arg ThreeAddressCode::add(Command command)
{
//	commands.push_back(command);
//	blocks.back().code.push_back(commands.size() - 1);

//	return Arg(IdType::TEMP, commands.size() - 1);

    Block& current_block = blocks[blockStack.top()];

    current_block.commands.push_back(command);
	current_block.code.push_back(current_block.commands.size() - 1);
	return Arg(IdType::TEMP, current_block.commands.size() - 1);
}

std::string ThreeAddressCode::toString()
{
	std::string res;
/*	
	for ( size_t i = 0; i < commands.size(); ++i )
	{
		if ( commands[i].op != SSAOp::PARAM && 
		     commands[i].op != SSAOp::RETURN && 
			 commands[i].op != SSAOp::IF && 
			 commands[i].op != SSAOp::IFFALSE &&
			 commands[i].op != SSAOp::GOTO &&
			 commands[i].op != SSAOp::ASSIGN &&
			 commands[i].op != SSAOp::LABEL )
		{
			res += "command " + std::to_string(i) + ": temp_" + std::to_string(i) + " = " + commands[i].toString() + '\n';
		}		
		else
		{
			res += "command " + std::to_string(i) + ": " + commands[i].toString() + '\n';
		}
	}
	res += '\n';
	res += "code:\n";*/

//	for ( auto i : code )
//		res += std::to_string(i) + '\n'; 

    for ( auto block : blocks )
    {
//        for ( auto command : block.code )
//            res += commands[command].toString() + "\n";
        res += block.toString() + "\n";
    }

	return res;
}
		
CodeObject ThreeAddressCode::genAsm() const
{
	CodeObject code_obj;
    
    for ( auto block : blocks )
    {
        block.genAsm(code_obj);
//    	for ( auto command_id : block.code )
//    		code_obj.emit(genCommand(commands[command_id]));
    }

	return code_obj;
}
/*
std::string ThreeAddressCode::genArg(Arg arg) const
{
	switch ( arg.type )
	{
		case IdType::STRING:
		{
			auto str_label = "string_label" + std::to_string(arg.id);
			
			std::string ans = "section .data\n";
			
			std::string res = "0";
			auto str = GlobalHelper::id_to_string[arg.id];
			for ( int i = str.length() - 1; i >= 0; --i )
			{
				if ( i >= 1 && str[i - 1] == '\\' && str[i] == 'n' )
				{
					res.append(", 10");
					--i;
				}
				else
					res.append(", " + std::to_string(static_cast<int>(str[i])));
			}

			ans.append("@" + str_label + ": db " + res + '\n')
			   .append(str_label + " equ $ - 1")
			   .append("section .text")
			   .append("lea rax, [" + str_label + "]");

			return ans;
		}
		case IdType::NUMBER:
		{
//			auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
//			return std::string("mov qword ").append(addr).append(", ").append(arg.toString() + '\n').append("lea rax, ").append(addr);
		}
		case IdType::TEMP:
		{
//			auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
//			std::string ans = genCommand(commands[arg.id]);
//
//			ans.append("\nmov ").append(addr).append(", rax\n").append("lea rax, ").append(addr);
//			return ans;
		}
        default:
            throw "IdType match failed in ThreeAddressCode::genArg";
	}
}

std::string ThreeAddressCode::genCommand(Command command) const
{
	switch ( command.op )
	{
		case SSAOp::PARAM:
		{
			
		}
		case SSAOp::CALL:
		{
				
		}
		case SSAOp::DOT:
		{
//			std::string res;
//			res.append(genArg(command.arg1));
//			
//			auto member = GlobalHelper::id_to_var[command.arg2.id];
//
//			res.append("lea rax, [rax - " + std::to_string(GlobalHelper::getOffset(member)) + "]");
//			return res;
		}
		default: //PLUS, MINUS, MUL, DIV, MOD, EQUALS, ELEM
		{
			return std::string().append(genArg(command.arg2))
			   .append("push qword [rax]")
			   .append(genArg(command.arg1))
			   .append("pop qword rdx")
			   .append("mov [rax], rdx");
		}
	}	
}
*/
void ThreeAddressCode::newBlock(Scope& scope)
{
    blocks.push_back(Block(scope));
    blockStack.push(blocks.size() - 1);
}

void ThreeAddressCode::popBlock()
{
    blockStack.pop();
}
