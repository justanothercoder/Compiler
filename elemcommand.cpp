#include "elemcommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "globaltable.hpp"
#include "scope.hpp"
#include "arraytype.hpp"
#include "globalconfig.hpp"
#include "builtins.hpp"
#include "typefactory.hpp"
#include "commandvisitor.hpp"

ElemCommand::ElemCommand(Arg* base, Arg* expr, bool is_string) : base(base), expr(expr), is_string(is_string) { }

void ElemCommand::gen(const Block& block, CodeObject& code_obj) const
{
    expr -> gen(block, code_obj);
    code_obj.emit("push qword [rax]");

    base -> gen(block, code_obj);
    code_obj.emit("pop rbx");
    
    if ( !is_string )
        code_obj.emit("imul rbx, " + std::to_string(static_cast<const ArrayType*>(base -> type()) -> type -> sizeOf()));
    
    code_obj.emit("add rax, rbx");

    code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");
}

std::string ElemCommand::toString() const { return base -> toString() + "[" + expr -> toString() + "]"; }

bool ElemCommand::isExpr() const { return true; }
    
const Type* ElemCommand::type() const 
{
    if ( is_string )
        return TypeFactory::getReference(BuiltIns::char_type);

    return TypeFactory::getReference(static_cast<const ArrayType*>(base -> type()) -> type);
}

void ElemCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
