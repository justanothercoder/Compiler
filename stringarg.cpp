#include "stringarg.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "builtins.hpp"

StringArg::StringArg(std::string str) : str(str)
{

}

void StringArg::gen(const Block& block, CodeObject& code_obj) const
{
    int string_id = block.table.id_by_string[str];
    code_obj.emit("lea rax, [string_label" + std::to_string(string_id) + "]");
}

std::string StringArg::toString() const
{
    return '"' + str + '"';
}

const Type* StringArg::type() const
{
    return BuiltIns::ASCII_string_type;
}
