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
#include "logger.hpp"
#include "globalconfig.hpp"
#include "inlinecallvisitor.hpp"

std::vector<CompilableUnit> Comp::units;
ThreeAddressCode Comp::code;

CompilableUnit& Comp::compile(std::string filename)
{
    if ( boost::optional<CompilableUnit&> mu = getUnit(filename) )
        return *mu;

    CompilableUnit unit(filename);

    auto root = FileHelper::parse((filename + ".txt").c_str());

    unit.module_symbol = std::make_shared<ModuleSymbol>(unit.module_name, BuiltIns::global_scope.get());
    root -> scope = unit.module_symbol;
    root -> build_scope();

    DefineVisitor define_visitor;
    root -> accept(define_visitor);

    ExpandTemplatesVisitor expand_visitor;
    root -> accept(expand_visitor);

    unit.module_globals = unit.module_symbol -> allSymbols();

    CheckVisitor check_visitor;
    root -> accept(check_visitor);

    InlineCallVisitor inline_call_visitor;
    root -> accept(inline_call_visitor);

    GenSSAVisitor visitor(Comp::code);
    root -> accept(visitor);

    unit.root = std::move(root);

    Comp::units.push_back(std::move(unit));
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
    
GlobalConfig& Comp::config()
{
    static GlobalConfig config;
    return config;
}
