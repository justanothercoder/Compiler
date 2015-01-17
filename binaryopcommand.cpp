#include "binaryopcommand.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "codeobject.hpp"
#include "builtins.hpp"
#include "commandvisitor.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "type.hpp"

BinaryOpCommand::BinaryOpCommand(BinaryOp op, Argument lhs, Argument rhs) : op_(op), lhs_(lhs), rhs_(rhs) { }

void BinaryOpCommand::gen(const Block& block, CodeObject& code_obj) const
{
    rhs_ -> gen(block, code_obj);

    if ( rhs_ -> type() -> isReference() )
        code_obj.emit("mov rax, [rax]");

    if ( rhs_ -> type() -> removeRef() == BuiltIns::char_type.get() )
    {
        code_obj.emit("mov qword [rsp], 0");
        code_obj.emit("mov bl, byte [rax]");
        code_obj.emit("sub rsp, " + std::to_string(Comp::config().int_size));
        code_obj.emit("mov byte [rsp], bl");
    }
    else
        code_obj.emit("push qword [rax]");

    lhs_ -> gen(block, code_obj);
    
    if ( lhs_ -> type() -> isReference() )
        code_obj.emit("mov rax, [rax]");

    code_obj.emit("pop rbx");

    if ( lhs_ -> type() -> removeRef() == BuiltIns::char_type.get() )
        code_obj.emit("mov rax, byte [rax]");
    else
        code_obj.emit("mov rax, [rax]");
    
    switch ( op_ )
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
                            
    code_obj.emit("mov [rbp - " + std::to_string(block.addressOf(this)) + "], rax");
}

std::string BinaryOpCommand::toString() const
{
    switch ( op_ )
    {
        case BinaryOp::PLUS   : return lhs_ -> toString() + " + "  + rhs_ -> toString();
        case BinaryOp::MINUS  : return lhs_ -> toString() + " - "  + rhs_ -> toString();
        case BinaryOp::MUL    : return lhs_ -> toString() + " * "  + rhs_ -> toString();
        case BinaryOp::DIV    : return lhs_ -> toString() + " / "  + rhs_ -> toString();
        case BinaryOp::MOD    : return lhs_ -> toString() + " % "  + rhs_ -> toString();
        case BinaryOp::EQUALS : return lhs_ -> toString() + " == " + rhs_ -> toString();
        case BinaryOp::NEQUALS: return lhs_ -> toString() + " != " + rhs_ -> toString();
        case BinaryOp::AND    : return lhs_ -> toString() + " && " + rhs_ -> toString();
        default: throw std::logic_error("internal error.");
    }
}

bool BinaryOpCommand::isExpr() const { return true; }
const Type* BinaryOpCommand::type() const { return BuiltIns::int_type.get(); }

void BinaryOpCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }
    
Arg* BinaryOpCommand::lhs() { return lhs_.get(); }
Arg* BinaryOpCommand::rhs() { return rhs_.get(); }

BinaryOp BinaryOpCommand::op() { return op_; }
