#include "functionhelper.hpp"

bool FunctionHelper::isCompatible(FunctionTypeInfo ft, const vector<Type*>& params_type)
{
    if ( ft.getNumberOfParams() != static_cast<int>(params_type.size()) )
	return false;
       
    for ( int i = 0; i < static_cast<int>(params_type.size()); ++i )
    {
	if ( !TypeHelper::isConvertable(params_type[i], ft.getParamType(i)) )
	    return false;
    }

    return true;
}

set<FunctionTypeInfo> FunctionHelper::getBestOverload(const set<FunctionTypeInfo>& selection, const vector<Type*>& params_type)
{
    set<FunctionTypeInfo> possible = selection;

    for ( auto i = std::begin(possible); i != std::end(possible); )
    {
	if ( !isCompatible(*i, params_type) )
	    i = possible.erase(i);
	else
	    ++i;
    }
    
/*    
    auto func_better = [&params_type](FunctionType *lhs, FunctionType *rhs)
    {
	
    }
*/        
    return possible;
}

FunctionSymbol* FunctionHelper::getViableOverload(OverloadedFunctionSymbol* overloaded_func, const vector<Type*>& params_type)
{
    auto overloads = FunctionHelper::getBestOverload(overloaded_func->getTypeInfo().overloads, params_type);

    if ( overloads.empty() )
	return nullptr;

    auto resolved_function_type_info = *std::begin(overloads);
    auto resolved_function_symbol = overloaded_func->getTypeInfo().symbols[resolved_function_type_info];

    return resolved_function_symbol;
}

void FunctionHelper::genCallCode(FunctionSymbol *func, const vector<ExprNode*>& params)
{
    int paramsSize = 0;    
    bool is_method = func->isMethod();

    auto resolved_function_type_info = func->getTypeInfo();

    int is_meth = (is_method ? 1 : 0);

    for ( int i = resolved_function_type_info.getNumberOfParams() - 1; i >= is_meth; --i )
    {
	params[i - is_meth]->gen();

	if ( dynamic_cast<ReferenceType*>(resolved_function_type_info.getParamType(i)) )
	{
	    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rax");
	    paramsSize += GlobalConfig::int_size;
	}
	else
	{
	    {
		auto conv = TypeHelper::getConversion(params[i - is_meth]->getType(), resolved_function_type_info.getParamType(i));

		CodeGen::emit("sub rsp, " + std::to_string(resolved_function_type_info.getParamType(i)->getSize()));
		
		int paramsSize = 0;
		for ( int j = 0; j < params[i - is_meth]->getType()->getSize(); j += GlobalConfig::int_size, paramsSize += GlobalConfig::int_size )
		{
		    CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		    CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rbx");
		}

		CodeGen::emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size * 10) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rbx");
		paramsSize += GlobalConfig::int_size;

		CodeGen::emit("sub rsp, " + std::to_string(paramsSize));
		CodeGen::emit("call _~" + conv->getScopedTypedName());
		CodeGen::emit("add rsp, " + std::to_string(paramsSize));
		
		CodeGen::emit("add rsp, " + std::to_string(resolved_function_type_info.getParamType(i)->getSize()));		
	    }

	    for ( int j = 0; j < resolved_function_type_info.getParamType(i)->getSize(); j += GlobalConfig::int_size, paramsSize += GlobalConfig::int_size )
	    {
		CodeGen::emit("mov rbx, [rax - " + std::to_string(j) + "]");
		CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rbx");
	    }
	}
    }

    if ( is_method )
    {
	CodeGen::emit("mov [rsp - " + std::to_string(paramsSize + GlobalConfig::int_size) + "], rdi");
	paramsSize += GlobalConfig::int_size;
    }

    CodeGen::emit("sub rsp, " + std::to_string(paramsSize));
    
    CodeGen::emit("call rsi");
    CodeGen::emit("add rsp, " + std::to_string(paramsSize));
}
