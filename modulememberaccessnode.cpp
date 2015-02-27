#include "modulememberaccessnode.hpp"
#include "symbol.hpp"
#include "varsymbol.hpp"
#include "functionalsymbol.hpp"

ModuleMemberAccessNode::ModuleMemberAccessNode(std::string name, std::string member) : name_(name), member_(member) { }

ASTNode ModuleMemberAccessNode::copyTree() const { return std::make_unique<ModuleMemberAccessNode>(name_, member_); }
std::string ModuleMemberAccessNode::toString() const { return name_ + "." + member_; }

void ModuleMemberAccessNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
    
VariableType ModuleMemberAccessNode::getType() const
{    
    if ( member_sym -> isVariable() ) {
        return static_cast<const VarSymbol*>(member_sym) -> typeOf();
    }
    else {
        return static_cast<const FunctionalSymbol*>(member_sym);
    }
}

bool ModuleMemberAccessNode::isLeftValue() const { return false; }

bool ModuleMemberAccessNode::isCompileTimeExpr() const { return false; } 
boost::optional<int> ModuleMemberAccessNode::getCompileTimeValue() const { return boost::none; }
    
const std::string& ModuleMemberAccessNode::name() const   { return name_; }
const std::string& ModuleMemberAccessNode::member() const { return member_; }

const Symbol* ModuleMemberAccessNode::memberSymbol() const { return member_sym; }
void ModuleMemberAccessNode::memberSymbol(Symbol* sym) { member_sym = sym; }

