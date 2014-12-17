#include "callcommand.hpp"
#include "functionsymbol.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "tempallocator.hpp"
#include "scope.hpp"
#include "commandvisitor.hpp"
#include "globalconfig.hpp"

CallCommand::CallCommand(const FunctionSymbol* function, int params_size) : function(function), params_size(params_size), is_inline_call(false)
{

}

void CallCommand::gen(const Block& block, CodeObject& code_obj) const
{
    if ( false && is_inline_call && !function -> isConstructor() )
    {
        block.alloc.remember(this, GlobalTable::transformAddress(&block.scope, block.scope.tempAlloc().getOffset()));
        block.scope.tempAlloc().claim(function -> type().returnType().sizeOf());

        auto& function_block = block.table.function_blocks.at(function);

        code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(this)) + "]");
        code_obj.emit("push rax"); 
        code_obj.emit("sub rsp, " + std::to_string(GlobalConfig::int_size));

        code_obj.emit("push rbp");
        code_obj.emit("mov rbp, rsp");

        if ( int var_space = function_block -> scope.varAlloc().getSpace() )
            code_obj.emit("sub rsp, " + std::to_string(var_space));

        if ( int temp_space = function_block -> scope.tempAlloc().getSpaceNeeded() )
            code_obj.emit("sub rsp, " + std::to_string(temp_space));

        for ( auto it = std::next(std::begin(function_block -> code)); it != std::end(function_block -> code); ++it )
            function_block -> commands[*it] -> gen(*function_block, code_obj);

        code_obj.emit("mov rsp, rbp");
        code_obj.emit("pop rbp");

        code_obj.emit("pop rax");
        code_obj.emit("add rsp, " + std::to_string(params_size + GlobalConfig::int_size));
        return;
    }

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

void CallCommand::setInlineCall()
{
    is_inline_call = true;
}
