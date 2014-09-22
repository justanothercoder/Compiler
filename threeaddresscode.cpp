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
	
