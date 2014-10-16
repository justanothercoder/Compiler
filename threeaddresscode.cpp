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
    Block& current_block = blocks[blockStack.top()];

    current_block.commands.push_back(command);
	current_block.code.push_back(current_block.commands.size() - 1);
	return Arg(IdType::TEMP, current_block.commands.size() - 1);
}

std::string ThreeAddressCode::toString()
{
	std::string res;
    for ( auto block : blocks )
        res += block.toString() + "\n";

	return res;
}
		
CodeObject ThreeAddressCode::genAsm() const
{
	CodeObject code_obj;
    
    for ( auto block : blocks )
        block.genAsm(code_obj);

	return code_obj;
}

void ThreeAddressCode::newBlock(Scope& scope)
{
    blocks.push_back(Block(scope));
    blockStack.push(blocks.size() - 1);
}

void ThreeAddressCode::popBlock()
{
    blockStack.pop();
}
