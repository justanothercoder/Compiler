#include "comp.hpp"
#include "filehelper.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definevisitor.hpp"
#include "checkvisitor.hpp"
#include "genssavisitor.hpp"
#include "ast.hpp"
#include "builtins.hpp"
#include "compilableunit.hpp"
#include "modulesymbol.hpp"
#include "optimizer.hpp"

std::vector<CompilableUnit> Comp::units;
ThreeAddressCode Comp::code;

CompilableUnit& Comp::compile(std::string filename)
{
    if ( boost::optional<CompilableUnit&> mu = getUnit(filename) )
        return *mu;

    CompilableUnit unit(filename);

    auto root = FileHelper::parse((filename + ".txt").c_str());
    unit.root = root;

    root -> scope = unit.module_symbol = new ModuleSymbol(unit.module_name, BuiltIns::global_scope);
    root -> build_scope();

    ExpandTemplatesVisitor expand_visitor;
    root -> accept(expand_visitor);

    DefineVisitor define_visitor;
    root -> accept(define_visitor);

    for ( auto sym : unit.module_symbol -> table )
        unit.module_globals.push_back(sym.second);

    CheckVisitor check_visitor;
    root -> accept(check_visitor);

    GenSSAVisitor visitor(Comp::code);
    root -> accept(visitor);

    Comp::units.push_back(unit);
    return Comp::units.back();
}
    
boost::optional<CompilableUnit&> Comp::getUnit(std::string module_name)
{
    for ( auto& unit : Comp::units )
    {
        if ( unit.module_name == module_name )
            return unit;
    }

    return boost::none;
}
