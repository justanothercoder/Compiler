#include "templatefunctionsymboldefine.hpp"
#include "globalscope.hpp"
#include "localscope.hpp"
#include "structscope.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "modulesymbol.hpp"
#include "overloadedfunctionsymbol.hpp"
#include "templatefunctionsymbol.hpp"

void TemplateFunctionSymbolDefine::setSymbol(std::shared_ptr<const Symbol> sym) { this -> sym = sym; }

void TemplateFunctionSymbolDefine::visit(ModuleSymbol* sc)  { visit(static_cast<BaseScope*>(sc)); }
void TemplateFunctionSymbolDefine::visit(GlobalScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void TemplateFunctionSymbolDefine::visit(LocalScope* sc)    { visit(static_cast<BaseScope*>(sc)); }
void TemplateFunctionSymbolDefine::visit(StructScope* sc)   { visit(static_cast<BaseScope*>(sc)); }
void TemplateFunctionSymbolDefine::visit(FunctionScope* sc) { visit(static_cast<BaseScope*>(sc)); }

void TemplateFunctionSymbolDefine::visit(BaseScope* sc)
{
    std::string sym_name = sym -> getName();

    auto it = sc -> table.find(sym_name);

    if ( it == std::end(sc -> table) )
//        sc -> table[sym_name] = new OverloadedFunctionSymbol(sym_name, OverloadedFunctionTypeInfo({ }), sym -> getTraits());
        sc -> table[sym_name] = std::make_shared<OverloadedFunctionSymbol>(sym_name, FunctionTraits::simple());

    auto _sym = sc -> table.at(sym_name);

    auto ofs = static_cast<const OverloadedFunctionSymbol*>(_sym.get());
    ofs -> setTemplateFunction(static_cast<const TemplateFunctionSymbol*>(sym.get()));
}
