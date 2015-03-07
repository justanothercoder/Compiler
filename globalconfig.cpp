#include "globalconfig.hpp"

GlobalConfig::GlobalConfig() : int_size(sizeof(int*))
{
    flags["fdumpTAC"]    = false;
    flags["optimize"]    = false;
    flags["fdumpmemory"] = false;
    flags["fnoinline"]   = false;
    flags["fcommentasm"] = true; // temporarily true
}

boost::optional<bool> GlobalConfig::flagValue(std::string flag) const
{
    auto it = flags.find(flag);

    if ( it == std::end(flags) )
        return boost::none;

    return it -> second;
}
    
void GlobalConfig::setFlag(std::string flag, bool value)
{
    flags.at(flag) = value;
}

std::string GlobalConfig::getCodeOperatorName(std::string op) const
{
    if      ( op == "operator()" ) return "operatorcall";
    else if ( op == "operator[]" ) return "operatorelem";
    else if ( op == "operator+"  ) return "operatorplus";
    else if ( op == "operator-"  ) return "operatorminus";
    else if ( op == "operator="  ) return "operatorassign";
    else if ( op == "operator*"  ) return "operatormul";
    else if ( op == "operator/"  ) return "operatordiv";
    else if ( op == "operator%"  ) return "operatormod";
    else if ( op == "operator!"  ) return "operatornot";
    else if ( op == "operator==" ) return "operatoreq";
    else if ( op == "operator!=" ) return "operatorneq";
    else if ( op == "operator&&" ) return "operatorand";
    else if ( op == "operator||" ) return "operatoror";
    else if ( op.substr(0, 8) == "operator" ) return "operator~" + op.substr(9);
    else throw;
}

