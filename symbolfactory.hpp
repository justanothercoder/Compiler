#ifndef _SYMBOLFACTORY_HPP_
#define _SYMBOLFACTORY_HPP_

#include <memory>
#include "symbol.hpp"
#include "functionalsymbol.hpp"
#include "varsymbol.hpp"
#include "typesymbol.hpp"
#include "functiontraits.hpp"
#include "functiontype.hpp"
#include "varinfo.hpp"

class StructScope;
class FunctionScope;
class FunctionDeclarationNode;

class SymbolFactory
{
public:

    std::unique_ptr<VarSymbol> makeVariable(std::string name, VariableType type, VariableSymbolType var_sym_type = VariableSymbolType::SIMPLE);
    std::unique_ptr<FunctionalSymbol> makeFunction(std::string    name
                                                 , FunctionType   type
                                                 , FunctionTraits traits
                                                 , bool           is_unsafe
                                                 , FunctionScope* scope = nullptr
                                                 , AST*           body = nullptr);

    std::unique_ptr<TypeSymbol> makeStruct(std::string name, StructScope* scope = nullptr);

    std::unique_ptr<TypeSymbol> makeLambda(const std::vector<VarSymbol*>& capture, const std::vector<VarInfo>& formal_params, AST* body);
};

#endif
