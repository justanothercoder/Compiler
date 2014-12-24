#include "returncommand.hpp"
#include "block.hpp"
#include "codeobject.hpp"
#include "builtins.hpp"
#include "referencetype.hpp"
#include "functionsymbol.hpp"
#include "commandvisitor.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"

ReturnCommand::ReturnCommand(Arg* expr, bool is_return_ref) : expr(expr), is_return_ref(is_return_ref)
{

}

void ReturnCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( is_return_ref )
    {
        expr -> gen(block, code_obj);
       
        code_obj.emit("mov rbx, [rax]"); 
        code_obj.emit("mov rax, [rbp + " + std::to_string(2 * Comp::config().int_size) + "]");
        code_obj.emit("mov [rax], rbx");
    }
    else
    {
        expr -> gen(block, code_obj);
       
        code_obj.emit("mov rbx, rax"); 
        code_obj.emit("mov rax, [rbp + " + std::to_string(2 * Comp::config().int_size) + "]");

        const Type *param_type = expr -> type();
           
        if ( param_type -> removeRef() == BuiltIns::int_type
          || param_type -> removeRef() -> getTypeKind() == TypeKind::POINTER )
        {
            if ( param_type -> isReference() )
                code_obj.emit("mov rbx, [rbx]");

            code_obj.emit("mov rcx, [rbx]");
            code_obj.emit("mov [rax], rcx");
        }
        else if ( param_type -> removeRef() == BuiltIns::char_type )
        {
            if ( param_type -> isReference() )
                code_obj.emit("mov rbx, [rbx]");

            code_obj.emit("mov cl, byte [rbx]");
            code_obj.emit("mov [rax], cl");
        }
        else
        {
            if ( param_type -> isReference() )
            {
                code_obj.emit("mov rbx, [rbx]");
                param_type = static_cast<const ReferenceType*>(param_type) -> type;
            }

            code_obj.emit("push rbx");
            code_obj.emit("push rax");

            assert(param_type -> getTypeKind() == TypeKind::STRUCT);

            code_obj.emit("call " + static_cast<const StructSymbol*>(param_type) -> getCopyConstructor() -> getScopedTypedName());
            code_obj.emit("add rsp, " + std::to_string(2 * Comp::config().int_size));
        }

        code_obj.emit("mov rsp, rbp");
        code_obj.emit("pop rbp");
        code_obj.emit("ret");
    }
}

std::string ReturnCommand::toString() const
{
    if ( is_return_ref ) return "returnref " + expr -> toString();
    else                 return "return "    + expr -> toString();
}

bool ReturnCommand::isExpr() const
{
    return false;
}

const Type* ReturnCommand::type() const 
{
    return nullptr;
}

void ReturnCommand::accept(CommandVisitor* visitor)
{
    visitor -> visit(this);
}
