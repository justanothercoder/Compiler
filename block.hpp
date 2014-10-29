#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <map>
#include <list>
#include <vector>
#include <unordered_map>

#include "command.hpp"

class Scope;
class CodeObject;

class VariableSymbol;
class FunctionSymbol;

class GlobalTable;

struct Block
{
    Block(Scope& scope, GlobalTable& table, std::string block_name = "");

    void genAsm(CodeObject& code_obj) const;

    void genArg(Arg arg, CodeObject& code_obj) const;
    void genCommand(Command command, CodeObject& code_obj) const;

    std::string toString();

    std::string toString(Arg arg) const;
    std::string toString(Command command) const;

    std::list<int> code;
    Scope& scope;

    std::vector<Command> commands;

    mutable std::unordered_map<Command, int> command_offsets;
    
    std::string block_name;

    GlobalTable& table;
};


#endif
