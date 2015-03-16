#include "modulenode.hpp"
#include "modulesymbol.hpp"

ModuleNode::ModuleNode(std::string name, ModuleSymbol* sym) : name_(name), outer_module(sym) { }

ASTNode ModuleNode::copyTree() const { return std::make_unique<ModuleNode>(name_); }

VariableType ModuleNode::getType() const { return nullptr; } 
bool ModuleNode::isLeftValue() const { return false; }

bool ModuleNode::isCompileTimeExpr() const { return false; } 
boost::optional<int> ModuleNode::getCompileTimeValue() const { return boost::none; } 

std::string ModuleNode::toString() const { return name_; }
void ModuleNode::accept(ASTVisitor& visitor) { visitor.visit(this); }

const std::string& ModuleNode::name() const { return name_; }

const ModuleSymbol* ModuleNode::module() const { return module_; }
void ModuleNode::module(const ModuleSymbol* module) { module_ = module; }
