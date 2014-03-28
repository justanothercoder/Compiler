#include "codegen.hpp"

void CodeGen::emit(string text) { std::cout << text << '\n'; }

void CodeGen::genConversion(FunctionSymbol *conv)
{
	auto par_type = conv->getTypeInfo().params_types[0];

	if ( conv->getName().substr(0, 8) == "operator" )
	{
		genCallCode(CallHelper::getCallInfo(conv, { }), { }, TemplateInfo(), [&] () {  });
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
//	param->gen(template_info);

//	if ( conv_info.deref )
//		emit("mov rax, [rax]");

//	if ( conv_info.conversion )
//		genConversion(conv_info.conversion);

	if ( copy_constr == nullptr ) 
	{
		param->gen(template_info);
		emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
		emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
	}
	else
	{
		if ( copy_constr == BuiltIns::int_copy_constructor )
		{
			param->gen(template_info);
			emit("mov rbx, [rax]");
			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
		}
		else
		{
			emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
			emit("sub rsp, " + std::to_string(param->getType().getSize()));
			genCallCode(CallHelper::getCallInfo(copy_constr, {param}), {param}, template_info, [&](){ emit("lea rax, [rbx]"); });
			emit("add rsp, " + std::to_string(param->getType().getSize()));
			auto desired_type = copy_constr->getTypeInfo().params_types[0];
			emit("sub rsp, " + std::to_string(desired_type.type->getSize())); 
		}
	}
/*
	if ( conv_info.ref )
	{
		emit("mov [rsp - 800], rax");
		emit("lea rax, [rsp - 800]");
	}
*/
}
