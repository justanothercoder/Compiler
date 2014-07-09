#include "codeobject.hpp"
#include "logger.hpp"

void CodeObject::gen() const { std::cout << code; }
std::string CodeObject::getCode() const { return code; }

void CodeObject::emit(string text) { code += text + '\n'; }

void CodeObject::pushOnStack(size_t size, int offset)
{
	for ( size_t i = 0; i < size; i += GlobalConfig::int_size )
	{
		emit("mov rbx, [rax - " + std::to_string(i) + "]");
		emit("mov [rsp - " + std::to_string(offset + i) + "], rbx");
	}
}
		
void CodeObject::genParam(ExprNode *param, ConversionInfo conv_info, FunctionSymbol *copy_constr)
{
	if ( conv_info.desired_type.is_ref )
	{
		emit(param -> gen().getCode());

		if ( conv_info.deref )
			emit("mov rax, [rax]");

		emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
		emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
	}
	else if ( conv_info.conversion )
	{
		auto conv = conv_info.conversion;

		if ( conv -> isOperator() )
			genCallCode(CallHelper::getCallInfo(conv, { }), { }, param -> gen(), param -> getType().is_ref);
		else
		{
			emit(param -> gen().getCode());
		
			emit("lea r8, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");

			emit("sub rsp, " + std::to_string(2 * GlobalConfig::int_size));		

			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));		

			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], r8");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

			emit("call " + conv -> getScopedTypedName());
			emit("add rsp, " + std::to_string(4 * GlobalConfig::int_size)); //offset + params + return

			emit("sub rsp, " + std::to_string(conv_info.desired_type.getSize()));
		}
	}
	else
	{
		if ( copy_constr == BuiltIns::int_copy_constructor )
		{
			emit(param -> gen().getCode());
			if ( conv_info.deref )
				emit("mov rax, [rax]");
			
			emit("mov rbx, [rax]");
			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
		}
		else
		{
			emit(param -> gen().getCode());
		
			emit("lea r8, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");

			emit("sub rsp, " + std::to_string(2 * GlobalConfig::int_size));		

			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));		

			emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], r8");
			emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

			emit("call " + copy_constr -> getScopedTypedName());
			emit("add rsp, " + std::to_string(4 * GlobalConfig::int_size)); //offset + params + return

//			emit("sub rsp, " + std::to_string(conv_info.desired_type.getSize()));
		}
	}
}

void CodeObject::genCallCode(CallInfo call_info, vector<ExprNode*> params, CodeObject& genThis, bool thisIsRef)
{
	auto func = call_info.callee;

	bool is_method = func -> isMethod();

	int is_meth = (is_method ? 1 : 0);

	auto function_info = func -> function_type_info;

	size_t params_size = std::accumulate(std::begin(function_info.params_types) + is_meth, std::end(function_info.params_types), 0, [](size_t x, VariableType type) { return x += type.getSize(); });

	if ( is_method )
		params_size += GlobalConfig::int_size;

	emit("sub rsp, " + std::to_string(func -> return_type.getSize()));

	for ( int i = static_cast<int>(function_info.params_types.size()) - 1; i >= is_meth; --i )
		genParam(params[i - is_meth], call_info.conversions[i - is_meth], call_info.copy_constructors[i - is_meth]);

	if ( is_method )
	{
		emit(genThis.getCode());
		if ( thisIsRef )
			emit("mov rax, [rax]");
		emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
		emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
	}

	emit("call " + func -> getScopedTypedName());
	emit("add rsp, " + std::to_string(params_size));
	emit("add rsp, " + std::to_string(func -> return_type.getSize()));
}

void CodeObject::genCopy(FunctionSymbol *copy_constr, CodeObject& genThis, CodeObject& copied)
{
	emit(copied.getCode());
	emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
	emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

	emit(genThis.getCode());
	emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
	emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

	emit("call " + copy_constr -> getScopedTypedName()); 
	emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size));
}

void CodeObject::genDefaultConstructorCall(FunctionSymbol *default_constr, CodeObject& genThis)
{
	emit(genThis.getCode());
	emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
	emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

	emit("call " + default_constr -> getScopedTypedName()); 
	emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size));
}
