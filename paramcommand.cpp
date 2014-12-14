#include "paramcommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "referencetype.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"

ParamCommand::ParamCommand(Arg* expr, ConversionInfo conversion_info) : expr(expr), conversion_info(conversion_info)
{

}

void ParamCommand::gen(const Block& block, CodeObject& code_obj) const
{
    expr -> gen(block, code_obj);

    const Type *param_type = expr -> type();
       
    if ( conversion_info.desired_type -> isReference() )
    {
        code_obj.emit("push rax");
    }
    else if ( param_type -> removeRef() == BuiltIns::int_type
           || param_type -> removeRef() -> getTypeKind() == TypeKind::POINTER )
    {
        if ( param_type -> isReference() )
            code_obj.emit("mov rax, [rax]");

        code_obj.emit("push qword [rax]");
    }
    else if ( param_type -> removeRef() == BuiltIns::char_type )
    {
        if ( param_type -> isReference() )
            code_obj.emit("mov rax, [rax]");

        code_obj.emit("xor rbx, rbx");
        code_obj.emit("mov bl, byte [rax]");
        code_obj.emit("push rbx");
    }
    else if ( conversion_info.conversion )
    {
        auto conv = conversion_info.conversion;

        code_obj.emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
        code_obj.emit("sub rsp, " + std::to_string(param_type -> sizeOf()));

        code_obj.emit("push rax");
        code_obj.emit("push rbx");

        code_obj.emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
        code_obj.emit("call " + conv -> getScopedTypedName());
        code_obj.emit("add rsp, " + std::to_string((2 + 1) * GlobalConfig::int_size));
    }
    else
    {
        code_obj.emit("lea rbx, [rsp - " + std::to_string(GlobalConfig::int_size) + "]");
        code_obj.emit("sub rsp, " + std::to_string(param_type -> sizeOf()));

        if ( param_type -> isReference() )
        {
            code_obj.emit("mov rax, [rax]");
            param_type = static_cast<const ReferenceType*>(param_type) -> type;
        }

        code_obj.emit("push rax");
        code_obj.emit("push rbx");

        code_obj.emit("call " + static_cast<const StructSymbol*>(param_type) -> getCopyConstructor() -> getScopedTypedName());
        code_obj.emit("add rsp, " + std::to_string(2 * GlobalConfig::int_size));
    }
}

std::string ParamCommand::toString() const
{
    return "param " + expr -> toString();
}

bool ParamCommand::isExpr() const
{
    return false;
}
