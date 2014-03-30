#include "codegen.hpp"

void CodeGen::emit(string text) { std::cout << text << '\n'; }

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
	emit("lea r9, [rbp - " + std::to_string(param->getScope()->get_valloc()->getAddressForLocal()) + "]");

	if ( copy_constr == nullptr ) 
	{
		param->gen(template_info);
		emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
		emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
	}
	else
	{
		auto conv = conv_info.conversion;

		if ( conv )
		{
			if ( conv->isOperator() )
				genCallCode(CallHelper::getCallInfo(conv, { }), { }, template_info, [&] () { param->gen(template_info); });
			else
				genCallCode(CallHelper::getCallInfo(conv, {param}), {param}, template_info, 
						[&] () { emit("lea rax, [rbp - " + std::to_string(param->getScope()->get_valloc()->getAddressForLocal()) + "]"); });
		}

		if ( copy_constr == BuiltIns::int_copy_constructor )
		{
			if ( conv == nullptr )
				param->gen(template_info);
			emit("mov rbx, [rax]");
			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
		}
		else
		{
			auto desired_type = copy_constr->getTypeInfo().params_types[0];
			
			emit("lea r8, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
			emit("sub rsp, " + std::to_string(param->getType().getSize()));
			if ( conv == nullptr )
				genCallCode(CallHelper::getCallInfo(copy_constr, {param}), {param}, template_info, [&](){ emit("lea rax, [r8]"); });
			else
			{
				pushOnStack(desired_type.type->getSize(), GlobalConfig::int_size);
				emit("call " + copy_constr->getScopedTypedName());
				emit("add rsp, " + std::to_string(desired_type.type->getSize())); 
			}
			emit("add rsp, " + std::to_string(param->getType().getSize()));
			emit("sub rsp, " + std::to_string(desired_type.type->getSize())); 
		}
	}

	emit("lea rax, [r9]");
}
