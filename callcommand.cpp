#include "callcommand.hpp"
#include "functionsymbol.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "scope.hpp"
#include "commandvisitor.hpp"
#include "globalconfig.hpp"

#include "builtins.hpp"

CallCommand::CallCommand(const FunctionalSymbol* function, int params_size) : params_size(params_size), function_(function) { }

void CallCommand::gen(const Block& block, CodeObject& code_obj) const
{
    code_obj.comment("Calling " + function_ -> getScopedTypedName());
    
    auto need_return_space = !function_ -> isConstructor() && function_ -> type().returnType() != BuiltIns::void_type;

    if ( need_return_space )
    {
        code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(this)) + "]");
        code_obj.emit("push rax");
    }

    code_obj.emit("call " + function_ -> getScopedTypedName());

    if ( need_return_space )
        code_obj.emit("pop rax");

    code_obj.emit("add rsp, " +  std::to_string(params_size));
}

std::string CallCommand::toString() const { return "call " + function_ -> getScopedTypedName() + ' ' + std::to_string(params_size); }

bool CallCommand::isExpr() const { return true; }
const Type* CallCommand::type() const { return function_ -> type().returnType().base(); }

void CallCommand::accept(CommandVisitor* visitor) { visitor -> visit(this); }

const FunctionalSymbol* CallCommand::function() const { return function_; }
