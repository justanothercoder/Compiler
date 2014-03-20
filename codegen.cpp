#include "codegen.hpp"

void CodeGen::emit(string text) { std::cout << text << '\n'; }

void CodeGen::genConversion(FunctionSymbol *conv)
{
	auto par_type = conv->getTypeInfo().params_types[0];

	if ( conv->getName().substr(0, 8) == "operator" )
	{
		genCallCode(CallHelper::getCallInfo(conv, { }), { }, TemplateInfo(),
				[&] () { emit("lea rax, [" + conv->getScopedTypedName() + "]"); },
				[&] () {  }
				);
	}
	else
	{
		int current_address = 0;

		pushOnStack(par_type.getSize(), GlobalConfig::int_size);

		current_address += par_type.getSize();

		emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size * 10) + "]");
		emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
		current_address += GlobalConfig::int_size;

		emit("sub rsp, " + std::to_string(current_address));
		emit("call " + conv->getScopedTypedName());
		emit("add rsp, " + std::to_string(current_address));
	}
}

void CodeGen::genCopy(FunctionSymbol *copy_constructor)
{
	int current_address = 0;

	auto type = copy_constructor->getTypeInfo().params_types[0];
	
	if ( type.is_ref )
		CodeGen::emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
	else
		pushOnStack(type.getSize(), GlobalConfig::int_size);

	current_address += type.getSize();

	emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
	emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
	current_address += GlobalConfig::int_size;

	emit("sub rsp, " + std::to_string(current_address));
	emit("call " + copy_constructor->getScopedTypedName());
	emit("add rsp, " + std::to_string(current_address));
}

void CodeGen::pushOnStack(size_t size, int offset)
{
	for ( size_t i = 0; i < size; i += GlobalConfig::int_size )
	{
		emit("mov rbx, [rax - " + std::to_string(i) + "]");
		emit("mov [rsp - " + std::to_string(offset + i) + "], rbx");
	}
}
		
void CodeGen::genParam(ExprNode *param, ConversionInfo conv_info, FunctionSymbol *copy_constr, const TemplateInfo& template_info)
{
	param->gen(template_info);

	if ( conv_info.deref )
		emit("mov eax, [eax]");

	if ( conv_info.conversion )
		genConversion(conv_info.conversion);

	if ( copy_constr == nullptr )
	{
		emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
		emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
	}
	else
	{
		if ( copy_constr == BuiltIns::int_copy_constructor )
		{
			emit("mov rbx, [rax]");
			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
		}
		else
		{
			genCopy(copy_constr);
			auto desired_type = copy_constr->getTypeInfo().params_types[0];
			emit("sub rsp, " + std::to_string(desired_type.getSize())); 
		}
	}

	if ( conv_info.ref )
	{
		emit("mov [rsp - 800], rax");
		emit("lea rax, [rsp - 800]");
	}
}
