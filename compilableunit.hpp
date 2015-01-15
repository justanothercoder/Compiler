#ifndef _COMPILABLEUNIT_HPP_
#define _COMPILABLEUNIT_HPP_

#include <string>
#include <vector>
#include <memory>

class AST;
class Symbol;
class ModuleSymbol;

class CompilableUnit
{
public:

    CompilableUnit(std::string module_name);

    std::string module_name;
    std::shared_ptr<AST> root = nullptr;
    std::shared_ptr<ModuleSymbol> module_symbol = nullptr;
    std::vector< std::shared_ptr<const Symbol> > module_globals;
};

#endif
