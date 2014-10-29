#include "typefactory.hpp"

#include "pointertype.hpp"
#include "referencetype.hpp"
#include "consttype.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"

#include "globaltable.hpp"

const Type* TypeFactory::getPointer(const Type *type)
{
	static std::map<const Type*, const Type*> pointers;

	if ( type -> getTypeKind() == TypeKind::REFERENCE )
		return nullptr;

	auto it = pointers.find(type);

	if ( it == std::end(pointers) )
	{
		pointers[type] = new PointerType(type);

		const auto& tp = pointers[type];
		auto tp_ref = getReference(tp);

//		auto pointer_assign = new FunctionSymbol("operator=", tp_ref, {tp_ref, tp}, BuiltIns::global_scope, {false, false, true});
		auto pointer_assign = new FunctionSymbol("operator=", tp_ref, {tp_ref, getConst(tp_ref)}, BuiltIns::global_scope, {false, false, true});
//		GlobalHelper::has_definition[pointer_assign] = true;
//        BuiltIns::global_scope -> getSymbolTable().has_definition[pointer_assign] = true;

		CodeObject assign_code;
		assign_code.emit(pointer_assign -> getScopedTypedName() + ":");
		
		assign_code.emit("push rbp");
		assign_code.emit("mov rbp, rsp");

		assign_code.emit("mov rax, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
		assign_code.emit("mov rbx, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
		assign_code.emit("mov rbx, [rbx]");
		assign_code.emit("mov [rax], rbx");

		assign_code.emit("mov rsp, rbp");
		assign_code.emit("pop rbp");
		assign_code.emit("ret");

		pointer_assign -> code_obj = assign_code;
//		assign_code.gen();

		BuiltIns::global_scope -> define(pointer_assign);

		auto pointer_add = new FunctionSymbol("operator+", tp, {tp, BuiltIns::int_type}, BuiltIns::global_scope, {false, false, true});
//		GlobalHelper::has_definition[pointer_add] = true;
//        BuiltIns::global_scope -> getSymbolTable().has_definition[pointer_add] = true;

		CodeObject add_code;
		add_code.emit(pointer_add -> getScopedTypedName() + ":");
		add_code.emit("push rbp");
		add_code.emit("mov rbp, rsp");

		add_code.emit("mov rbx, [rbp + " + std::to_string(2 * GlobalConfig::int_size) + "]");
		add_code.emit("mov rax, [rbp + " + std::to_string(3 * GlobalConfig::int_size) + "]");
		add_code.emit("add rbx, rax");

		add_code.emit("mov rax, [rbp + " + std::to_string(4 * GlobalConfig::int_size) + "]");
		add_code.emit("mov [rax], rbx");

		add_code.emit("mov rsp, rbp");
		add_code.emit("pop rbp");
		add_code.emit("ret");

		pointer_add -> code_obj = add_code;
//		add_code.gen();

		BuiltIns::global_scope -> define(pointer_add);
	}

	return pointers[type];
}

const Type* TypeFactory::getReference(const Type *type)
{
	static std::map<const Type*, const Type*> references;
	
	if ( type -> getTypeKind() == TypeKind::REFERENCE )
		return nullptr;

	auto it = references.find(type);

	if ( it == std::end(references) )
		references[type] = new ReferenceType(type);

	return references[type];
}

const Type* TypeFactory::getConst(const Type *type)
{
	static std::map<const Type*, const Type*> consts;

	if ( type -> getTypeKind() == TypeKind::CONSTTYPE )
		return type;

	auto it = consts.find(type);

	if ( it == std::end(consts) )
		consts[type] = new ConstType(type);

	return consts[type];
}
