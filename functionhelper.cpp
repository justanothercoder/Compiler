#include "functionhelper.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "typefactory.hpp"
#include "builtintypesymbol.hpp"
/*
FunctionSymbol* FunctionHelper::makeDefaultCopyConstructor(StructSymbol *struc)
{
	auto ref_struc       = TypeFactory::getReference(struc);
	auto const_ref_struc = TypeFactory::getConst(ref_struc);

	auto copy_constr = new FunctionSymbol(struc -> getName(), ref_struc, {ref_struc, const_ref_struc}, struc, {true, true, false});

	GlobalHelper::has_definition[copy_constr] = true;

	boost::optional<CodeObject> func_code = CodeObject();

	func_code -> emit("jmp _~" + copy_constr -> getScopedTypedName());
	func_code -> emit(copy_constr -> getScopedTypedName() + ":");
	func_code -> emit("push rbp");
	func_code -> emit("mov rbp, rsp");

	for ( auto member : struc -> table )
	{
//		if ( dynamic_cast<VariableSymbol*>(member.second) )
		if ( member.second -> getSymbolType() == SymbolType::VARIABLE )		
		{	
			auto var = static_cast<VariableSymbol*>(member.second); 
			auto var_type = var -> getType();

			if ( dynamic_cast<const OverloadedFunctionSymbol*>(var_type) || dynamic_cast<const BuiltInTypeSymbol*>(var_type) )
				continue;

			auto member_copy = static_cast<const StructSymbol*>(var_type) -> getCopyConstructor();

			CodeObject param;
			param.emit("mov rax, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
			param.emit("lea rax, [rax - " + std::to_string(struc -> getVarAlloc().getAddress(var)) + "]");

			CodeObject genThis;
			genThis.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
			genThis.emit("lea rax, [rax - " + std::to_string(struc -> getVarAlloc().getAddress(var)) + "]");

			func_code -> genCopy(member_copy, genThis, param); 
		}
	}

	func_code -> emit("mov rsp, rbp");
	func_code -> emit("pop rbp");
	func_code -> emit("ret");
	func_code -> emit("_~" + copy_constr -> getScopedTypedName() + ":");

	copy_constr -> code_obj = func_code;

	return copy_constr;
}

FunctionSymbol* FunctionHelper::makeDefaultConstructor(StructSymbol *struc)
{
	auto ref_struc = TypeFactory::getReference(struc);

	auto constr = new FunctionSymbol(struc -> getName(), ref_struc, {ref_struc}, struc, {true, true, false});

	GlobalHelper::has_definition[constr] = true;

	boost::optional<CodeObject> func_code = CodeObject();

	func_code -> emit("jmp _~" + constr -> getScopedTypedName());
	func_code -> emit(constr -> getScopedTypedName() + ":");
	func_code -> emit("push rbp");
	func_code -> emit("mov rbp, rsp");

	for ( auto member : struc -> table )
	{
//		if ( dynamic_cast<VariableSymbol*>(member.second) )
		if ( member.second -> getSymbolType() == SymbolType::VARIABLE )
		{	
			auto var = static_cast<VariableSymbol*>(member.second); 
			auto var_type = var -> getType();

			if ( dynamic_cast<const OverloadedFunctionSymbol*>(var_type) || dynamic_cast<const BuiltInTypeSymbol*>(var_type) )
				continue;

			auto member_default = static_cast<const StructSymbol*>(var_type) -> getDefaultConstructor();

			if ( member_default == nullptr )
				throw SemanticError(var_type -> getName() + " doesn't have default constructor");

			CodeObject genThis;
			genThis.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
			genThis.emit("lea rax, [rax - " + std::to_string(struc -> getVarAlloc().getAddress(var)) + "]");

			func_code -> genDefaultConstructorCall(member_default, genThis); 
		}
	}

	func_code -> emit("mov rsp, rbp");
	func_code -> emit("pop rbp");
	func_code -> emit("ret");
	func_code -> emit("_~" + constr -> getScopedTypedName() + ":");

	constr -> code_obj = func_code;

	return constr;
}
*/
