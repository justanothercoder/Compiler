#include "copytypevisitor.hpp"
#include "codeobject.hpp"
#include "consttype.hpp"
#include "structsymbol.hpp"
#include "builtintypesymbol.hpp"
#include "functionsymbol.hpp"
	
CopyTypeVisitor::CopyTypeVisitor(std::string from, std::string to) : from(from), to(to)
{

}

void CopyTypeVisitor::visit(const ConstType *tp)
{
	tp -> type -> accept(*this);
}

void CopyTypeVisitor::visit(const PointerType *) 
{
	code_obj = *(new CodeObject());	

	code_obj.emit("lea r14, [" + from + "]");
	code_obj.emit("lea r15, [" + to + "]");
	code_obj.emit("mov r13, [r14]");
	code_obj.emit("mov [r15], r13");
}

void CopyTypeVisitor::visit(const StructSymbol *tp)
{
	code_obj = *(new CodeObject());	
	
	auto copy_constr = tp -> getCopyConstructor();

	code_obj.emit("lea r14, [" + from + "]");
	code_obj.emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], r14");
	code_obj.emit("sub rsp, " + std::to_string(GlobalConfig::int_size));		

	code_obj.emit("lea r15, [" + to + "]");
	code_obj.emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], r15");
	code_obj.emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

	code_obj.emit("call " + copy_constr -> getScopedTypedName());
	code_obj.emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size)); //offset + params 
}

void CopyTypeVisitor::visit(const ReferenceType *)
{
	code_obj = *(new CodeObject());	

	code_obj.emit("lea r14, [" + from + "]");
	code_obj.emit("lea r15, [" + to + "]");
	code_obj.emit("mov [r15], r14");
}

void CopyTypeVisitor::visit(const FunctionSymbol *)
{
	code_obj = *(new CodeObject());	
	throw SemanticError("Can't copy OverloadedFunctionSymbol");
}

void CopyTypeVisitor::visit(const BuiltInTypeSymbol *tp)
{
	code_obj = *(new CodeObject());

	code_obj.emit("lea r14, [" + from + "]");
	code_obj.emit("lea r15, [" + to   + "]");

	for ( size_t i = 0, tp_size = tp -> getSize(); i < tp_size; i += GlobalConfig::int_size )
	{
		code_obj.emit("mov r13, [r14 - " + std::to_string(i) + "]");		
		code_obj.emit("mov [r15 - " + std::to_string(i) + "], r13");		
	}
}

void CopyTypeVisitor::visit(const OverloadedFunctionSymbol *)
{
	code_obj = *(new CodeObject());	
	throw SemanticError("Can't copy OverloadedFunctionSymbol");
}

CodeObject CopyTypeVisitor::getCopyCode(const Type *type)
{
	type -> accept(*this);
	return code_obj;
}
