#include "threeaddresscode.hpp"

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
	commands.push_back(command);
	code.push_back(commands.size() - 1);

	return Arg(IdType::TEMP, code.back());
}

std::string ThreeAddressCode::toString()
{
	std::string res;
	
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
	res += "code:\n";

	for ( auto i : code )
		res += std::to_string(i) + '\n'; 

//	for ( auto command : code )
//		res += commands[command].toString() + "\n";
	return res;
}
		
CodeObject ThreeAddressCode::genAsm() const
{
	CodeObject code_obj;

	for ( auto command_id : code )
		code_obj.emit(genCommand(commands[command_id]));

	return code_obj;
}

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
			auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
			return std::string("mov qword ").append(addr).append(", ").append(arg.toString() + '\n').append("lea rax, ").append(addr);
		}
		case IdType::TEMP:
		{
			auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
			std::string ans = genCommand(commands[arg.id]);

			ans.append("\nmov ").append(addr).append(", rax\n").append("lea rax, ").append(addr);
			return ans;
		}
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
			std::string res;
			res.append(genArg(command.arg1));
			
			auto member = GlobalHelper::id_to_var[command.arg2.id];

			res.append("lea rax, [rax - " + std::to_string(GlobalHelper::getOffset(member)) + "]");
			return res;
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
	
void ThreeAddressCode::pushScope(VarAllocator alloc)
{
	var_alloc.push(alloc);
	temp_alloc.push(TempAllocator());
}

void ThreeAddressCode::popScope()
{
	var_alloc.pop();
	temp_alloc.pop();
}

