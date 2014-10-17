#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <list>
#include <vector>
#include <unordered_map>

#include "command.hpp"

class Scope;
class CodeObject;

struct Block
{
    Block(Scope& scope, std::string block_name = "");

    void genAsm(CodeObject& code_obj);

    void genArg(Arg arg, CodeObject& code_obj) const;
    void genCommand(Command command, CodeObject& code_obj) const;

    std::string toString();

    std::list<int> code;
    Scope& scope;

    std::vector<Command> commands;

    mutable std::unordered_map<Command, int> command_offsets;
    
    std::string block_name;
};


#endif
