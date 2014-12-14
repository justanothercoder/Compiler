#include "binaryopcommand.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "codeobject.hpp"
#include "builtins.hpp"

BinaryOpCommand::BinaryOpCommand(BinaryOp op, Arg* lhs, Arg* rhs) : op(op), lhs(lhs), rhs(rhs)
{

}

void BinaryOpCommand::gen(const Block& block, CodeObject& code_obj) const
{
    block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
    block.scope.tempAlloc().claim(GlobalConfig::int_size);

    rhs -> gen(block, code_obj);

    if ( rhs -> type() -> isReference() )
        code_obj.emit("mov rax, [rax]");

    if ( rhs -> type() -> removeRef() == BuiltIns::char_type )
    {
        code_obj.emit("mov qword [rsp], 0");
        code_obj.emit("mov bl, byte [rax]");
        code_obj.emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
        code_obj.emit("mov byte [rsp], bl");
    }
    else
        code_obj.emit("push qword [rax]");

    lhs -> gen(block, code_obj);
    
    if ( lhs -> type() -> isReference() )
        code_obj.emit("mov rax, [rax]");

    code_obj.emit("pop rbx");

    if ( lhs -> type() -> removeRef() == BuiltIns::char_type )
        code_obj.emit("mov rax, byte [rax]");
    else
        code_obj.emit("mov rax, [rax]");
    
    switch ( op )
    {
        case BinaryOp::PLUS   : code_obj.emit("add rax, rbx"); break;
        case BinaryOp::MINUS  : code_obj.emit("sub rax, rbx"); break;
        case BinaryOp::MUL    : code_obj.emit("imul rbx"); break;
        case BinaryOp::DIV    : code_obj.emit("xor rdx, rdx"); code_obj.emit("idiv rbx"); break;
        case BinaryOp::MOD    : code_obj.emit("xor rdx, rdx"); code_obj.emit("idiv rbx"); code_obj.emit("mov rax, rdx"); break;
        case BinaryOp::EQUALS : code_obj.emit("cmp rax, rbx"); code_obj.emit("mov rax, qword 0"); code_obj.emit("sete al"); break;
        case BinaryOp::NEQUALS: code_obj.emit("cmp rax, rbx"); code_obj.emit("mov rax, qword 0"); code_obj.emit("setne al"); break;
        case BinaryOp::AND    : 
        {
            code_obj.emit("cmp rax, 0"); 
            code_obj.emit("setne al");
            
            code_obj.emit("cmp rbx, 0");
            code_obj.emit("setne bl");

            code_obj.emit("test al, bl");
            code_obj.emit("setnz al");
            break;
        }
        default:
           throw std::logic_error("internal error.");
    }
                            
    code_obj.emit("mov [rbp - " + std::to_string(block.alloc.addressOf(this)) + "], rax");
}

std::string BinaryOpCommand::toString() const
{
    switch ( op )
    {
        case BinaryOp::PLUS   : return lhs -> toString() + " + "  + rhs -> toString();
        case BinaryOp::MINUS  : return lhs -> toString() + " - "  + rhs -> toString();
        case BinaryOp::MUL    : return lhs -> toString() + " * "  + rhs -> toString();
        case BinaryOp::DIV    : return lhs -> toString() + " / "  + rhs -> toString();
        case BinaryOp::MOD    : return lhs -> toString() + " % "  + rhs -> toString();
        case BinaryOp::EQUALS : return lhs -> toString() + " == " + rhs -> toString();
        case BinaryOp::NEQUALS: return lhs -> toString() + " != " + rhs -> toString();
        case BinaryOp::AND    : return lhs -> toString() + " && " + rhs -> toString();
        default: throw std::logic_error("internal error.");
    }
}

bool BinaryOpCommand::isExpr() const
{
    return true;
}
