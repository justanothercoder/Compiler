#include "newcommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "globaltable.hpp"
#include "scope.hpp"
#include "tempallocator.hpp"
#include "type.hpp"
    
NewCommand::NewCommand(const Type* type) : type(type)
{

}

void NewCommand::gen(const Block& block, CodeObject&) const
{
    block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
    block.scope.tempAlloc().claim(type -> sizeOf());
}

std::string NewCommand::toString() const
{
    return "new " + type -> getName();
}

bool NewCommand::isExpr() const
{
    return true;
}    
