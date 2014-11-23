#ifndef _COMPILABLEUNIT_HPP_
#define _COMPILABLEUNIT_HPP_

#include <string>
#include <vector>

class AST;
class Symbol;
class ModuleSymbol;

class CompilableUnit
{
public:

    CompilableUnit(std::string module_name);

    std::string module_name;
    AST* root;
    ModuleSymbol* module_symbol;
    std::vector<Symbol*> module_globals;
};

#endif
