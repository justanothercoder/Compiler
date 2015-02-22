#ifndef _STRUCTSYMBOL_HPP_
#define _STRUCTSYMBOL_HPP_

#include <map>

#include "scope.hpp"
#include "symbol.hpp"
#include "objecttype.hpp"
#include "structscope.hpp"
#include "functiontype.hpp"
#include "typesymbol.hpp"

class VariableSymbol;

class StructSymbol : public TypeSymbol
{
public:

    StructSymbol(std::string name, StructScope* scope);

    std::string getName() const override;
    std::string typeName() const override;
    
    size_t sizeOf() const override;

    const FunctionalSymbol* getConversionTo(const Type *type) const;
    bool isConvertableTo(const Type *type) const override;

    boost::optional<int> rankOfConversion(const Type *type) const override;

    bool isType() const override;

    Scope* innerScope() const override;
    
    std::vector<VarSymbol*> members() const;
    std::vector<FunctionalSymbol*> methods() const;
    
    void defineMethod(std::unique_ptr<FunctionalSymbol> method) override;
    void defineMember(std::unique_ptr<VarSymbol> member) override;
    
private:

    std::string name;
    StructScope* scope;
};

#endif
