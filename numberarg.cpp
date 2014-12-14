#include "numberarg.hpp"
#include "codeobject.hpp"
#include "block.hpp"
#include "globaltable.hpp"
#include "builtins.hpp"

NumberArg::NumberArg(int value) : value(value)
{

}

void NumberArg::gen(const Block& block, CodeObject& code_obj) const
{
    int num_id = block.table.const_num_id[value];
    code_obj.emit("lea rax, [iconst" + std::to_string(num_id) + "]");
}

std::string NumberArg::toString() const
{
    return std::to_string(value);
}

const Type* NumberArg::type() const
{
    return BuiltIns::int_type;
}
