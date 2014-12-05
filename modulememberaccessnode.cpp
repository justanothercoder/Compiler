#include "modulememberaccessnode.hpp"
#include "symbol.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "overloadedfunctionsymbol.hpp"

ModuleMemberAccessNode::ModuleMemberAccessNode(std::string name
                                             , std::string member) : name      (name)
                                                                   , member    (member)
                                                                   , member_sym(nullptr)
{

}

AST* ModuleMemberAccessNode::copyTree() const 
{
    return new ModuleMemberAccessNode(name, member);
}

std::string ModuleMemberAccessNode::toString() const 
{
    return name + "." + member;
}

void ModuleMemberAccessNode::accept(ASTVisitor& visitor) 
{
    visitor.visit(this);
}
    
const Type* ModuleMemberAccessNode::getType() const
{    
    if ( member_sym -> getSymbolType() == SymbolType::VARIABLE ) 
        return static_cast<VariableSymbol*>(member_sym) -> getType();
    return static_cast<const OverloadedFunctionSymbol*>(member_sym);
}

bool ModuleMemberAccessNode::isLeftValue() const
{
    return false;
}

bool ModuleMemberAccessNode::isCompileTimeExpr() const
{
    return false;
}

boost::optional<int> ModuleMemberAccessNode::getCompileTimeValue() const 
{
    return boost::none;
}
