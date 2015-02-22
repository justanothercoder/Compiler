#ifndef _GLOBALTABLE_HPP_
#define _GLOBALTABLE_HPP_

#include <map>
#include <string>
#include <unordered_map>

class Scope;
class FunctionSymbol;
struct Block;

class GlobalTable
{
public:

    static int transformAddress(Scope *scope, int addr);

    std::map<const FunctionalSymbol*, bool> has_definition;

    std::map<int, int> const_num_id;
    std::map<int, int> id_to_num;

    std::map<std::string, int> id_by_string;
    std::map<int, std::string> string_by_id;

    std::map<const FunctionalSymbol*, Block*> function_blocks;
};

#endif
