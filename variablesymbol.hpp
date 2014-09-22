#ifndef _VARIABLESYMBOL_HPP_
#define _VARIABLESYMBOL_HPP_

#include "symbol.hpp"

class Type;

enum class VariableSymbolType { SIMPLE, PARAM, FIELD };

class VariableSymbol : public Symbol
{
public:

    VariableSymbol(std::string name, const Type *type, VariableSymbolType sym_type = VariableSymbolType::SIMPLE);

    const Type* getType() const;

    void setType(const Type *t);

    std::string getName() const override;

    bool isParam() const;
    bool isField() const;

    SymbolType getSymbolType() const override;

	ScopeVisitor& getScopeVisitor() override;
	
private:

	std::string name;
    const Type *type;

    VariableSymbolType sym_type;
};

#endif
