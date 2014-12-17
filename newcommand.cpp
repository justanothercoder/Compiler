#include "newcommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "globaltable.hpp"
#include "scope.hpp"
#include "tempallocator.hpp"
#include "type.hpp"
#include "commandvisitor.hpp"
    
NewCommand::NewCommand(const Type* _type) : _type(_type) { }

std::string NewCommand::toString() const { return "new " + _type -> getName(); }

bool NewCommand::isExpr() const { return true; }    
const Type* NewCommand::type() const { return _type; }

void NewCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
void NewCommand::gen(const Block&, CodeObject&) const { } 
