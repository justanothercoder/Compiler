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
class VarSymbol;
class FunctionalSymbol;

class GlobalTable;

struct Block
{
    friend class Optimizer;

public:
    Block(Scope* scope, GlobalTable& table, std::string block_name = "");

    void computeMemoryDisposition() const;
    void genAsm(CodeObject& code_obj) const;

    void addCommand(std::shared_ptr<Command> command);

    int addressOf(const VarSymbol* var) const;
    int addressOf(const Command* command) const;

    void allocate(const VarSymbol* sym) const;

    Command* commandById(int command_id);
    int numId(int num) const;
    int stringId(const std::string& str) const;

    std::string toString();    

    Scope* scope() const;
    std::list<int>& code();

private:

    std::list<int> code_;
    Scope* scope_;

    std::vector< std::shared_ptr<Command> > commands;
    std::string block_name;
    GlobalTable& table;

    mutable SpaceAllocator alloc;
};


#endif
