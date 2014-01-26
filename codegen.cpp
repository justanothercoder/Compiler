#include "codegen.hpp"

void CodeGen::emit(string text)
{
    cout << text << '\n';
}

void CodeGen::construct_object(Type *type, FunctionSymbol *constructor, const vector<ExprNode*>& params)
{
    CodeGen::emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rdi");
    CodeGen::emit("sub rsp, " + std::to_string(type->getSize()));
    
    CodeGen::emit("push rsi");
    CodeGen::emit("lea rsi, [" + constructor->getScopedTypedName() + "]");

    CodeGen::genCallCode(constructor, params);

    CodeGen::emit("pop rsi");
}

void CodeGen::genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params)
{
    int params_size = 0, current_address = 0;
    bool is_method = func->isMethod();
    bool is_operator = func->isOperator();

    int is_meth = ((is_method && !is_operator) ? 1 : 0);

    auto resolved_function_type_info = func->getTypeInfo();

    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
	params_size += resolved_function_type_info.getParamType(i)->getSize();

    if ( is_method && !is_operator )
	params_size += GlobalConfig::int_size;
    
    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
	params[i - is_meth]->gen();

	if ( resolved_function_type_info.getParamType(i)->isReference() )
	{
	    CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rax");
	    current_address += GlobalConfig::int_size;
	}
	else
	{
	    if ( params[i - is_meth]->getType() != resolved_function_type_info.getParamType(i) )
	    {
		bool refconv = params[i - is_meth]->getType()->isReference() &&
		    static_cast<ReferenceType*>(params[i - is_meth]->getType())->getReferredType() == resolved_function_type_info.getParamType(i);

		Type *par_type = params[i - is_meth]->getType();
		
		if ( refconv )
		    par_type = static_cast<ReferenceType*>(par_type)->getReferredType();

		auto conv = TypeHelper::getConversion(par_type, resolved_function_type_info.getParamType(i));
		CodeGen::emit("sub rsp, " + std::to_string(params_size));
		
		int current_address = 0;
		for ( int j = 0; j < par_type->getSize(); j += GlobalConfig::int_size, current_address += GlobalConfig::int_size )
		{
		    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		    CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
		}

		CodeGen::emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size * 10) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
		current_address += GlobalConfig::int_size;

		if ( !refconv )
		{
		    CodeGen::emit("sub rsp, " + std::to_string(current_address));
		    CodeGen::emit("call _~" + conv->getScopedTypedName());
		    CodeGen::emit("add rsp, " + std::to_string(current_address));
		}
		CodeGen::emit("add rsp, " + std::to_string(params_size));
	    }
	}

	for ( int j = 0; j < resolved_function_type_info.getParamType(i)->getSize(); j += GlobalConfig::int_size, current_address += GlobalConfig::int_size )
	{
	    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
	    CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rbx");
	}
    }

    if ( is_method && !is_operator )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(current_address + GlobalConfig::int_size) + "], rdi");
	current_address += GlobalConfig::int_size;
    }

    CodeGen::emit("sub rsp, " + std::to_string(params_size));
    
    CodeGen::emit("call rsi");
    CodeGen::emit("add rsp, " + std::to_string(params_size));
}
