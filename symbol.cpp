#include "symbol.hpp"
#include "symboldefine.hpp"

Symbol::~Symbol()
{

}

ScopeVisitor& Symbol::getScopeVisitor()
{
    return *(new SymbolDefine(this));
}
