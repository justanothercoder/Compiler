#include "block.hpp"
#include "scope.hpp"
#include "codeobject.hpp"
#include "variablesymbol.hpp"
#include "functionsymbol.hpp"
#include "structsymbol.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"
#include "referencetype.hpp"
#include "arraytype.hpp"
#include "labelcommand.hpp"
#include "globalconfig.hpp"
#include "disposememoryvisitor.hpp"

#include "logger.hpp"

Block::Block(Scope& scope, GlobalTable& table, std::string block_name) : scope     (scope)
                                                                       , block_name(block_name)
                                                                       , table     (table)
                                                                       , alloc     ((1 + (dynamic_cast<FunctionScope*>(&scope) ? !dynamic_cast<FunctionScope*>(&scope) -> func -> isConstructor() : 0)) * GlobalConfig::int_size)
{ 
    if ( dynamic_cast<FunctionScope*>(&scope) )
        table.function_blocks[static_cast<FunctionScope&>(scope).func] = this;
}

std::string Block::toString()
{
    std::string res = "";

    for ( auto command : code )
        res += commands[command] -> toString() + "\n";

    return res;
}

void Block::computeMemoryDisposition() const
{
    DisposeMemoryVisitor disposer(alloc);
    for ( auto command_id : code )
    {
        auto command = commands[command_id];
        command -> accept(&disposer);
    }
}

void Block::genAsm(CodeObject& code_obj) const
{
    if ( code.empty() )
        return;

    auto it = std::begin(code);

    if ( dynamic_cast<LabelCommand*>(commands[*it]) )
    {
        commands[*it] -> gen(*this, code_obj);
        ++it;
    }

    if ( it == std::end(code) )
        return;

    code_obj.emit("push rbp");
    code_obj.emit("mov rbp, rsp");

    if ( int total_space = alloc.totalSpaceUsed() )
        code_obj.emit("sub rsp, " + std::to_string(total_space));

    for ( ; it != std::end(code); ++it )
        commands[*it] -> gen(*this, code_obj);

    code_obj.emit("mov rsp, rbp");
    code_obj.emit("pop rbp");
}
