#include "temporaryarg.hpp"
#include "block.hpp"
#include "codeobject.hpp"

int TemporaryArg::temp_id = 0;

TemporaryArg::TemporaryArg(Command* command) : command(command)
{
    ++temp_id;
}

void TemporaryArg::gen(const Block& block, CodeObject& code_obj) const
{
    code_obj.emit("lea rax, [rbp - " + std::to_string(block.alloc.addressOf(command)) + "]");
}

std::string TemporaryArg::toString() const
{
    return "temp_" + std::to_string(temp_id);
}
