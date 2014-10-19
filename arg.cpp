#include "arg.hpp"
#include "globalhelper.hpp"
#include "functionsymbol.hpp"
#include "variablesymbol.hpp"

Arg::Arg(IdType type, int id, const Type *expr_type) : type(type), id(id), expr_type(expr_type)
{

}
	
std::string Arg::toString() const
{
    switch ( type )
    {
    case IdType::NOID     : return "noid";
    case IdType::STRING   : return '"' + GlobalHelper::string_by_id[id] + '"';
    case IdType::NUMBER   : return std::to_string(GlobalHelper::id_to_num[id]);
    case IdType::TEMP     : return "temp_" + std::to_string(id);
    case IdType::LABEL    : return GlobalHelper::label_name[id] + ":";
    case IdType::PROCEDURE: return GlobalHelper::func_by_id[id] -> getScopedTypedName();
    case IdType::VARIABLE : return GlobalHelper::var_by_id[id] -> getName();
    }
}

bool Arg::operator==(const Arg& a) const
{
    return type == a.type && id == a.id;
}
