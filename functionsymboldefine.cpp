#include "functionsymboldefine.hpp"

FunctionSymbolDefine::FunctionSymbolDefine(FunctionSymbol *sym) : sym(sym) { }

void FunctionSymbolDefine::visit(GlobalScope *sc) { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(LocalScope *sc) { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(StructSymbol *sc) { visit(static_cast<BaseScope*>(sc)); }
void FunctionSymbolDefine::visit(FunctionSymbol *sc) { visit(static_cast<BaseScope*>(sc)); }

void FunctionSymbolDefine::visit(BaseScope *sc)
{
	string sym_name = sym->getName();

	auto it = sc->table.find(sym_name);

	if ( it == std::end(sc->table) )
		sc->table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym->getTraits()));

	Symbol *_sym = sc->table[sym_name];

	if ( _sym->getSymbolType() != SymbolType::VARIABLE )
		throw SemanticError(sym_name + " is already defined.");

	if ( static_cast<VariableSymbol*>(_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
		throw SemanticError(sym_name + " is already defined as not function");

	auto func_type_info = sym->getTypeInfo();

	auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(_sym)->getType());
	ofs->addOverload(func_type_info, sym);
}
