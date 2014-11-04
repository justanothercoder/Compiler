#include "arg.hpp"
#include "globaltable.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "builtins.hpp"

#include <iostream>
#include <cassert>

Arg::Arg(IdType type, int id) : type(type), id(id), expr_type(nullptr)
{
    assert(type == IdType::NOID || type == IdType::LABEL || type == IdType::PROCEDURE);
}

Arg::Arg(IdType type, int id, const Type *expr_type) : type(type), id(id), expr_type(expr_type)
{
    assert(expr_type != nullptr);
}

bool Arg::operator==(const Arg& a) const
{
    return type == a.type && id == a.id;
}
