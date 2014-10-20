#include "threeaddresscode.hpp"
#include "globalhelper.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"

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
    Block& current_block = blocks[blockStack.top()];

    const Type *command_type;

    switch ( command.op )
    {
        case SSAOp::ASSIGN: case SSAOp::PARAM: case SSAOp::LABEL: case SSAOp::RETURN: case SSAOp::IF:
        case SSAOp::IFFALSE: case SSAOp::GOTO: 
            command_type = nullptr;
            break;
        case SSAOp::PLUS: case SSAOp::MINUS: case SSAOp::MUL: case SSAOp::DIV: case SSAOp::MOD: case SSAOp::EQUALS:
            command_type = BuiltIns::int_type;
            break;
        case SSAOp::ELEM:
            command_type = nullptr;
            break;
        case SSAOp::DEREF:
            command_type = nullptr;
            break;
        case SSAOp::ADDR:
            command_type = nullptr;
            break;
        case SSAOp::DOT:
            command_type = GlobalHelper::var_by_id[command.arg2.id] -> getType();
            break;
        case SSAOp::CALL:
            command_type = GlobalHelper::func_by_id[command.arg1.id] -> return_type;
            break;        
        default:
            throw "internal error.";
    }

    current_block.commands.push_back(command);
	current_block.code.push_back(current_block.commands.size() - 1);
	return Arg(IdType::TEMP, current_block.commands.size() - 1, command_type);
}

std::string ThreeAddressCode::toString()
{
	std::string res;
    for ( auto block : blocks )
        res += block.toString() + "\n";

	return res;
}
		
void ThreeAddressCode::genAsm(CodeObject& code_obj) const
{
    auto block = blocks.cbegin();

    for ( auto p : GlobalHelper::has_definition )
    {
        if ( !p.second )
            code_obj.emit("extern " + p.first -> getScopedTypedName());
    }

    code_obj.emit("section .text");
    code_obj.emit("global _start");
    code_obj.emit("_start:");

    block -> genAsm(code_obj);

    for ( ++block; block != blocks.cend(); ++block )
    {
        code_obj.emit("jmp _~" + block -> block_name);
        block -> genAsm(code_obj);
        code_obj.emit("ret");
        code_obj.emit("_~" + block -> block_name + ":");
    }

    code_obj.emit("mov rax, 60");
    code_obj.emit("mov rdi, 0");
    code_obj.emit("syscall");
}

void ThreeAddressCode::newBlock(Scope& scope, std::string block_name)
{
    if ( block_name == "" )
        block_name = scope.getScopeName();

    blocks.push_back(Block(scope, block_name));
    blockStack.push(blocks.size() - 1);
}

void ThreeAddressCode::popBlock()
{
    blockStack.pop();
}
