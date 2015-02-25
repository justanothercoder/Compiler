#include "block.hpp"
#include "scope.hpp"
#include "codeobject.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"
#include "referencetype.hpp"
#include "arraytype.hpp"
#include "labelcommand.hpp"
#include "globalconfig.hpp"
#include "functionscope.hpp"
#include "functionalsymbol.hpp"
#include "disposememoryvisitor.hpp"
#include "comp.hpp"
#include "logger.hpp"

Block::Block(Scope* scope, GlobalTable& table, std::string block_name) : scope_    (scope)
                                                                       , block_name(block_name)
                                                                       , table     (table)
                                                                       , alloc     ((1 + (dynamic_cast<FunctionScope*>(scope) ? !dynamic_cast<FunctionScope*>(scope) -> func -> isConstructor() : 0)) * Comp::config().int_size)
{ 
    if ( dynamic_cast<FunctionScope*>(scope) )
        table.function_blocks[static_cast<FunctionScope*>(scope) -> func] = this;
}

std::string Block::toString()
{
    std::string res = "";

    for ( auto command : code_ )
        res += commands[command] -> toString() + "\n";

    return res;
}

void Block::computeMemoryDisposition() const
{
    DisposeMemoryVisitor disposer(alloc);
    for ( auto command_id : code_ )
    {
        auto command = commands[command_id];
        command -> accept(&disposer);
    }

    if ( *Comp::config().flagValue("fdumpmemory") )
        Logger::log(alloc.dumpDisposition());
}

void Block::genAsm(CodeObject& code_obj) const
{
    if ( code_.empty() )
        return;

    auto it = std::begin(code_);

    if ( dynamic_cast<LabelCommand*>(commands[*it].get()) )
    {
        commands[*it] -> gen(*this, code_obj);
        ++it;
    }

    if ( it == std::end(code_) )
        return;

    code_obj.emit("push rbp");
    code_obj.emit("mov rbp, rsp");

    if ( int total_space = alloc.totalSpaceUsed() )
        code_obj.emit("sub rsp, " + std::to_string(total_space));

    for ( ; it != std::end(code_); ++it )
        commands[*it] -> gen(*this, code_obj);

    code_obj.emit("mov rsp, rbp");
    code_obj.emit("pop rbp");
}

void Block::addCommand(std::shared_ptr<Command> command)
{
    commands.push_back(command);
    code_.push_back(commands.size() - 1);
}

void Block::allocate(const VarSymbol* var) const
{
    alloc.remember(var);
}
    
int Block::addressOf(const VarSymbol* var) const { return alloc.addressOf(var); }
int Block::addressOf(const Command* command)    const { return alloc.addressOf(command); }

Command* Block::commandById(int command_id) { return commands.at(command_id).get(); }
int Block::numId(int num) const { return table.const_num_id.at(num); }
int Block::stringId(const std::string& str) const { return table.id_by_string.at(str); }

Scope* Block::scope() const { return scope_; }
std::list<int>& Block::code() { return code_; }
