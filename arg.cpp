#include "arg.hpp"
#include "globaltable.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"
#include "builtins.hpp"

#include <iostream>

Arg::Arg(IdType type, int id) : type(type), id(id), expr_type(nullptr)
{
    if ( !(type == IdType::NOID || type == IdType::LABEL || type == IdType::PROCEDURE) )
        throw std::logic_error("Internal error.");
}

Arg::Arg(IdType type, int id, const Type *expr_type) : type(type), id(id), expr_type(expr_type)
{
    if ( expr_type == nullptr )
        throw std::logic_error("nullptr error.");
}

bool Arg::operator==(const Arg& a) const
{
    return type == a.type && id == a.id;
}
