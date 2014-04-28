#include "functionhelper.hpp"

bool FunctionHelper::isCompatible(FunctionTypeInfo ft, vector<VariableType> params_type)
{
	if ( ft.params_types.size() != params_type.size() )
		return false;	

	for ( size_t i = 0; i < params_type.size(); ++i )
	{
		if ( !TypeHelper::isConvertable(params_type[i], ft.params_types[i]) )
			return false;
	}

	return true;
}

set<FunctionTypeInfo> FunctionHelper::getBestOverload(set<FunctionTypeInfo> selection, vector<VariableType> params_type)
{
	auto possible = selection;

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

FunctionSymbol* FunctionHelper::getViableOverload(OverloadedFunctionSymbol *overloaded_func, vector<VariableType> params_type)
{
	auto overloads = FunctionHelper::getBestOverload(overloaded_func->getTypeInfo().overloads, params_type);
	return overloads.empty() ? nullptr : overloaded_func->getTypeInfo().symbols.at(*std::begin(overloads)); 
}


FunctionSymbol* FunctionHelper::makeDefaultCopyConstructor(StructSymbol *struc, const TemplateInfo& template_info)
{
	auto copy_constr = new FunctionSymbol(struc->getName(),
		 								  FunctionTypeInfo(VariableType(struc, true),
										                  {VariableType(struc, true),
														   VariableType(struc, true, true)}),
										  struc,
										  {true, true, false}, 
										  nullptr
	);

	CodeObject *func_code = new CodeObject();

	func_code->emit("jmp _~" + copy_constr->getScopedTypedName());
	func_code->emit(copy_constr->getScopedTypedName() + ":");
	func_code->emit("push rbp");
	func_code->emit("mov rbp, rsp");

	for ( auto member : struc->table )
	{
		if ( dynamic_cast<VariableSymbol*>(member.second) )
		{	
			auto var = dynamic_cast<VariableSymbol*>(member.second); 
			auto var_type = var->getType();

			if ( dynamic_cast<OverloadedFunctionSymbol*>(var_type.type) )
				continue;

			auto member_copy = TypeHelper::getCopyConstructor(var_type);

			CodeObject param;
			param.emit("mov rax, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
			param.emit("lea rax, [rax - " + std::to_string(struc->get_valloc()->getAddress(var)) + "]");

			CodeObject genThis;
			genThis.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
			genThis.emit("lea rax, [rax - " + std::to_string(struc->get_valloc()->getAddress(var)) + "]");

			func_code->genCopy(member_copy, genThis, param); 
		}
	}

	func_code->emit("mov rsp, rbp");
	func_code->emit("pop rbp");
	func_code->emit("ret");
	func_code->emit("_~" + copy_constr->getScopedTypedName() + ":");

	copy_constr->code_obj = func_code;

	return copy_constr;
}

FunctionSymbol* FunctionHelper::makeDefaultConstructor(StructSymbol *struc, const TemplateInfo& template_info)
{
	auto constr = new FunctionSymbol(struc->getName(),
		 						     FunctionTypeInfo(VariableType(struc, true),
									                 {VariableType(struc, true)}),
                                     struc,
									 {true, true, false}, 
									 nullptr
	);

	CodeObject *func_code = new CodeObject();

	func_code->emit("jmp _~" + constr->getScopedTypedName());
	func_code->emit(constr->getScopedTypedName() + ":");
	func_code->emit("push rbp");
	func_code->emit("mov rbp, rsp");

	for ( auto member : struc->table )
	{
		if ( dynamic_cast<VariableSymbol*>(member.second) )
		{	
			auto var = dynamic_cast<VariableSymbol*>(member.second); 
			auto var_type = var->getType();

			if ( dynamic_cast<OverloadedFunctionSymbol*>(var_type.type) )
				continue;

			auto member_default = TypeHelper::getDefaultConstructor(var_type);

			if ( member_default == nullptr )
				throw SemanticError(var_type.getName() + " doesn't have default constructor");

			CodeObject genThis;
			genThis.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
			genThis.emit("lea rax, [rax - " + std::to_string(struc->get_valloc()->getAddress(var)) + "]");

			func_code->genDefaultConstructorCall(member_default, genThis); 
		}
	}

	func_code->emit("mov rsp, rbp");
	func_code->emit("pop rbp");
	func_code->emit("ret");
	func_code->emit("_~" + constr->getScopedTypedName() + ":");

	constr->code_obj = func_code;

	return constr;
}
