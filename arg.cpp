#include "arg.hpp"
#include "globaltable.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "builtins.hpp"

Arg::Arg(IdType type, int id, const Type *expr_type) : type(type), id(id), expr_type(expr_type)
{

}

bool Arg::operator==(const Arg& a) const
{
    return type == a.type && id == a.id;
}
