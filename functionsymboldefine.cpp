#include "functionsymboldefine.hpp"

FunctionSymbolDefine::FunctionSymbolDefine(FunctionSymbol *sym) : sym(sym)
{
    
}

void FunctionSymbolDefine::visit(GlobalScope *sc)
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

void FunctionSymbolDefine::visit(LocalScope *sc)
{
    string sym_name = sym->getName();
    
    auto it = sc->table.find(sym_name);
	
    if ( it == std::end(sc->table) )
	sc->table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym->getTraits()));

    Symbol *_sym = sc->table[sym_name];	

    if ( _sym->getSymbolType() != SymbolType::VARIABLE || static_cast<VariableSymbol*>(_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	throw SemanticError(sym_name + " is already defined.");

    auto func_type_info = sym->getTypeInfo();

    auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(_sym)->getType());
    ofs->addOverload(func_type_info, sym);
}

void FunctionSymbolDefine::visit(StructSymbol *sc)
{
    string sym_name = sym->getName();
	
    auto it = sc->table.find(sym_name);
	
    if ( it == std::end(sc->table) )
	sc->table[sym_name] = new VariableSymbol(sym_name, new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym->getTraits()));

    Symbol *_sym = sc->table[sym_name];	

    if ( _sym->getSymbolType() != SymbolType::VARIABLE || static_cast<VariableSymbol*>(_sym)->getType()->getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
	throw SemanticError(sym_name + " is already defined.");

    auto func_type_info = sym->getTypeInfo();

    auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(_sym)->getType());
    ofs->addOverload(func_type_info, sym);	
}

void FunctionSymbolDefine::visit(FunctionSymbol *sc)
{
    string sym_name = sym->getName();

    if ( sc->table[sym_name] == nullptr )
	sc->table[sym_name] = new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym->getTraits());

    if ( sc->table[sym_name]->getSymbolType() != SymbolType::OVERLOADED_FUNCTION )
	throw SemanticError(sym_name + " is not a function");

    OverloadedFunctionSymbol* ov_func = static_cast<OverloadedFunctionSymbol*>(sc->table[sym_name]);	
	
    auto type_info = ov_func->getTypeInfo();	
    auto func_type_info = sym->getTypeInfo();

    auto ofs = static_cast<OverloadedFunctionSymbol*>(static_cast<VariableSymbol*>(sc->table[sym_name])->getType());
    ofs->addOverload(func_type_info, sym);

}
