#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <map>
#include <list>
#include <vector>
#include <unordered_map>

#include "command.hpp"
#include "spaceallocator.hpp"

class Scope;
class CodeObject;

class VariableSymbol;
class FunctionSymbol;

class GlobalTable;

struct Block
{
    Block(Scope& scope, GlobalTable& table, std::string block_name = "");

    void computeMemoryDisposition() const;
    void genAsm(CodeObject& code_obj) const;

    std::string toString();

    std::list<int> code;
    Scope& scope;

    std::vector<Command*> commands;

    std::string block_name;

    GlobalTable& table;

    mutable SpaceAllocator alloc;
};


#endif
