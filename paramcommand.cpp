#include "paramcommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "referencetype.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"
#include "commandvisitor.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

#include "logger.hpp"

ParamCommand::ParamCommand(Argument expr, ConversionInfo conversion_info) : expr_(expr), conversion_info(conversion_info) { }

void ParamCommand::gen(const Block& block, CodeObject& code_obj) const
{
    expr_ -> gen(block, code_obj);

    auto param_type = expr_ -> type();

    if ( conversion_info.desired_type -> isReference() )
    {
        code_obj.emit("push rax");
    }
    else if ( isIntType(param_type -> removeRef()) || param_type -> removeRef() -> isPointer() )
    {
        if ( param_type -> isReference() )
            code_obj.emit("mov rax, [rax]");

        code_obj.emit("push qword [rax]");
    }
    else if ( isCharType(param_type -> removeRef()) )
    {
        if ( param_type -> isReference() )
            code_obj.emit("mov rax, [rax]");

        code_obj.emit("xor rbx, rbx");
        code_obj.emit("mov bl, byte [rax]");
        code_obj.emit("push rbx");
    }
    else if ( auto conv = conversion_info.conversion )
    {
        code_obj.emit("lea rbx, [rsp - " + std::to_string(Comp::config().int_size) + "]");
        code_obj.emit("sub rsp, " + std::to_string(param_type -> sizeOf()));

        code_obj.emit("push rax");
        code_obj.emit("push rbx");

        code_obj.emit("call " + conv -> getScopedTypedName());
        code_obj.emit("add rsp, " + std::to_string(2 * Comp::config().int_size));
    }
    else
    {
        code_obj.emit("lea rbx, [rsp - " + std::to_string(Comp::config().int_size) + "]");
        code_obj.emit("sub rsp, " + std::to_string(param_type -> sizeOf()));

        if ( param_type -> isReference() )
        {
            code_obj.emit("mov rax, [rax]");
            param_type = param_type -> removeRef();
        }

        code_obj.emit("push rax");
        code_obj.emit("push rbx");

        code_obj.emit("call " + static_cast<const StructSymbol*>(param_type) -> getCopyConstructor() -> getScopedTypedName());
        code_obj.emit("add rsp, " + std::to_string(2 * Comp::config().int_size));
    }
}

std::string ParamCommand::toString() const { return "param " + expr_ -> toString(); }

bool ParamCommand::isExpr() const { return false; }
const Type* ParamCommand::type() const { return nullptr; }

void ParamCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }

Arg* ParamCommand::expr() { return expr_.get(); }
const ConversionInfo& ParamCommand::conversionInfo() const { return conversion_info; }

