#include "callcommand.hpp"
#include "functionsymbol.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "tempallocator.hpp"
#include "scope.hpp"
#include "commandvisitor.hpp"

CallCommand::CallCommand(const FunctionSymbol* function, int params_size) : function(function), params_size(params_size)
{

}

void CallCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( function -> isConstructor() )
    {
        code_obj.emit("call " + function -> getScopedTypedName());
        code_obj.emit("add rsp, " + std::to_string(params_size));
    }
    else
    {
        block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
        block.scope.tempAlloc().claim(function -> type().returnType().sizeOf());

        code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(this)) + "]");
        code_obj.emit("push rax");

        code_obj.emit("call " + function -> getScopedTypedName());
        code_obj.emit("pop rax");
        code_obj.emit("add rsp, " +  std::to_string(params_size));
    }
}

std::string CallCommand::toString() const
{
    return "call " + function -> getScopedTypedName() + ' ' + std::to_string(params_size);
}

bool CallCommand::isExpr() const
{
    return true;
}

const Type* CallCommand::type() const
{
    return function -> type().returnType().base();
}

void CallCommand::accept(CommandVisitor* visitor)
{
    visitor -> visit(this);
}
