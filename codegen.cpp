#include "codegen.hpp"

void CodeGen::emit(string text) { std::cout << text << '\n'; }

void CodeGen::construct_object(FunctionSymbol *constructor, const vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*> template_expr)
{
	CodeGen::emit("push rsi");
	CodeGen::emit("lea rsi, [" + constructor->getScopedTypedName() + "]");

	CodeGen::genCallCode(constructor, params, template_sym, template_expr);

	CodeGen::emit("pop rsi");
}

void CodeGen::genConversion(FunctionSymbol *conv)
{
	int current_address = 0;

	auto par_type = conv->getTypeInfo().getParamType(0);

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

void CodeGen::genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*> template_expr)
{
	int params_size = 0;
	int current_address = 0;

	bool is_method = func->isMethod();

	int is_meth = (is_method ? 1 : 0);

	auto resolved_function_type_info = func->getTypeInfo();

	auto& pt = resolved_function_type_info.getParamsTypes();

	params_size = std::accumulate(std::begin(pt) + is_meth, std::end(pt), 0, [](int x, Type *type) { return x += type->getSize(); });

	if ( is_method )
		params_size += GlobalConfig::int_size;

	for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
	{
		params[i - is_meth]->gen(template_sym, template_expr);

		Type *param_type = resolved_function_type_info.getParamType(i);

		if ( param_type->isReference() )
		{
			CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rax");
			current_address += GlobalConfig::int_size;
		}
		else
		{
			if ( params[i - is_meth]->getType() != param_type )
			{
				bool refconv = params[i - is_meth]->getType()->isReference() &&
					static_cast<ReferenceType*>(params[i - is_meth]->getType())->getReferredType() == param_type;

				Type *par_type = params[i - is_meth]->getType();

				if ( refconv )
					par_type = static_cast<ReferenceType*>(par_type)->getReferredType();
				else
				{
					auto conv = TypeHelper::getConversion(par_type, param_type);

					CodeGen::genConversion(conv);
				}
			}
			auto copy_constr = TypeHelper::getCopyConstructor(param_type);	
			CodeGen::genCopy(copy_constr, current_address, param_type);
			current_address += param_type->getSize();
		}
	}

	if ( is_method )
		CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rdi");

	CodeGen::emit("sub rsp, " + std::to_string(params_size));

	CodeGen::emit("call rsi");
	CodeGen::emit("add rsp, " + std::to_string(params_size));
}
