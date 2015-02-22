#include "stringarg.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "builtins.hpp"

StringArg::StringArg(std::string str) : str(str) { }

void StringArg::gen(const Block& block, CodeObject& code_obj) const
{
    code_obj.emit("lea rax, [string_label" + std::to_string(block.stringId(str)) + "]");
}

std::string StringArg::toString() const { return '"' + str + '"'; }
const Type* StringArg::type() const { return BuiltIns::ASCII_string_type; }
