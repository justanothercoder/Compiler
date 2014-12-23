#include "dotcommand.hpp"
#include "structsymbol.hpp"
#include "variablearg.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "temporaryarg.hpp"
#include "commandvisitor.hpp"

DotCommand::DotCommand(Arg* expr, int offset, VariableSymbol* member) : expr(expr), offset(offset), member(member)
{

}

void DotCommand::gen(const Block& block, CodeObject& code_obj) const
{
   auto base_type = expr -> type();

   if ( base_type -> isReference() )
   {
       expr -> gen(block, code_obj);

       code_obj.emit("lea rax, [rax + " + std::to_string(offset) + "]");
       code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");
   }
}

std::string DotCommand::toString() const { return expr -> toString() + "." + member -> getName(); }

bool DotCommand::isExpr() const { return true; }
const Type* DotCommand::type() const { return member -> getType().base(); }

void DotCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
