#include "temporaryarg.hpp"
#include "block.hpp"
#include "type.hpp"
#include "codeobject.hpp"

int TemporaryArg::temp_id = 0;

TemporaryArg::TemporaryArg(std::shared_ptr<Command> command) : command_(command) { ++temp_id; }

void TemporaryArg::gen(const Block& block, CodeObject& code_obj) const
{
    code_obj.emit("lea rax, [rbp - " + std::to_string(block.addressOf(command_.get())) + "]");
}

std::string TemporaryArg::toString() const { return "temp(" + command_ -> toString() + ")"; } 
const Type* TemporaryArg::type() const { return command_ -> type(); }
    
Command* TemporaryArg::command() const { return command_.get(); }
