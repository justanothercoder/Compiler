#ifndef _GLOBALTABLE_HPP_
#define _GLOBALTABLE_HPP_

#include <map>
#include <string>
#include <unordered_map>

#include "templateparam.hpp"
#include "conversioninfo.hpp"

class Type;
class Scope;
class FunctionSymbol;
class VariableSymbol;

struct Block;

class GlobalTable
{
public:

    static int transformAddress(Scope *scope, int addr);

    std::map<const FunctionSymbol*, bool> has_definition;

    std::map<int, int> const_num_id;
    std::map<int, int> id_to_num;

    std::map<std::string, int> id_by_string;
    std::map<int, std::string> string_by_id;

    std::map<const FunctionSymbol*, Block*> function_blocks;
};

#endif
