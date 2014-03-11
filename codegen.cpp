#include "codegen.hpp"

void CodeGen::emit(string text) { std::cout << text << '\n'; }

void CodeGen::genConversion(FunctionSymbol *conv)
{
	int current_address = 0;

	auto par_type = conv->getTypeInfo().params_types[0];

	pushOnStack(par_type->getSize(), GlobalConfig::int_size);

	current_address += par_type->getSize();

	CodeGen::emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size * 10) + "]");
	CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
	current_address += GlobalConfig::int_size;

	CodeGen::emit("sub rsp, " + std::to_string(current_address));
	CodeGen::emit("call " + conv->getScopedTypedName());
	CodeGen::emit("add rsp, " + std::to_string(current_address));
}

void CodeGen::genCopy(FunctionSymbol *copy_constructor, int stack_offset, Type *type)
{
	int current_address = 0;

	auto par_type = copy_constructor->getTypeInfo().params_types[0];
	
	if ( par_type->isReference() )
		CodeGen::emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
	else
		pushOnStack(type->getSize(), GlobalConfig::int_size);

	current_address += type->getSize();

	CodeGen::emit("lea rbx, [rsp - " + std::to_string(stack_offset + GlobalConfig::int_size) + "]");
	CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
	current_address += GlobalConfig::int_size;

	CodeGen::emit("sub rsp, " + std::to_string(current_address));
	CodeGen::emit("call " + copy_constructor->getScopedTypedName());
	CodeGen::emit("add rsp, " + std::to_string(current_address));
}

void CodeGen::pushOnStack(unsigned int size, int offset)
{
	for ( unsigned int i = 0; i < size; i += GlobalConfig::int_size )
	{
		CodeGen::emit("mov rbx, [rax - " + std::to_string(i) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(offset + i) + "], rbx");
	}
}
