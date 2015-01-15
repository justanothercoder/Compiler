#include "threeaddresscode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"
#include "globaltable.hpp"
#include "typefactory.hpp"
#include "pointertype.hpp"

#include "labelarg.hpp"
#include "temporaryarg.hpp"

#include "logger.hpp"

Argument ThreeAddressCode::newLabel(const std::string& label)
{
    static int label_num = 0;
    return std::make_shared<LabelArg>(label != "" ? label : "label_" + std::to_string(++label_num));
}

Argument ThreeAddressCode::add(std::shared_ptr<Command> command)
{
    Block* current_block = blocks[blockStack.top()];

    current_block -> addCommand(command);

    return std::make_shared<TemporaryArg>(command);
}

std::string ThreeAddressCode::toString()
{
    std::string res;
    for ( auto block : blocks )
        res += block -> toString() + "\n";

    return res;
}

void ThreeAddressCode::computeMemoryDisposition() const
{
    for ( auto block : blocks )
        block -> computeMemoryDisposition();
}

void ThreeAddressCode::genAsm(CodeObject& code_obj) const
{
    computeMemoryDisposition();

    for ( auto p : globaltable.has_definition )
    {
        if ( !p.second && p.first -> is_used )
            code_obj.emit("extern " + p.first -> getScopedTypedName());
    }

    code_obj.emit("section .data");

    for ( auto p : globaltable.id_to_num )
        code_obj.emit("iconst" + std::to_string(p.first) + ": dq " + std::to_string(p.second));

    for ( auto p : globaltable.string_by_id )
    {
        auto str_label = "string_label" + std::to_string(p.first);

        std::string res = "";
        const auto& str = p.second;
       
        if ( !str.empty() )
        { 
            size_t i = 0;
            if ( i < str.length() - 1 && str[i] == '\\' && str[i + 1] == 'n' )
            {
                res.append("10");
                ++i;
            }
            else
            {
                res.append(std::to_string(static_cast<int>(str[i])));
            }
            for ( ++i; i < str.length(); ++i )
            {
                if ( i < str.length() - 1 && str[i] == '\\' && str[i + 1] == 'n' )
                {
                    res.append(", 10");
                    ++i;
                }
                else
                {
                    res.append(", " + std::to_string(static_cast<int>(str[i])));
                }
            }
            res.append(", 0");
        }
        else
        {
            res = "0";
        }

        code_obj.emit(str_label + ": db " + res);
    }

    code_obj.emit("section .text");

    auto mainblock = blocks.cbegin();

    for ( auto block = blocks.cbegin(); block != blocks.cend(); ++block )
    {
        if ( !dynamic_cast<FunctionScope*>((*block) -> scope()) )
        {
            mainblock = block;
            continue;
        }

        if ( dynamic_cast<FunctionScope*>((*block) -> scope()) -> func -> is_used )
        {
            (*block) -> genAsm(code_obj);
            code_obj.emit("ret");
        }
    }
    
    code_obj.emit("global _start");
    code_obj.emit("_start:");

    (*mainblock) -> genAsm(code_obj);    

    code_obj.emit("mov rax, 60");
    code_obj.emit("mov rdi, 0");
    code_obj.emit("syscall");
}

void ThreeAddressCode::newBlock(Scope* scope, std::string block_name)
{
    if ( block_name == "" )
        block_name = scope -> getScopeName();

    blocks.push_back(new Block(scope, globaltable, block_name));
    blockStack.push(blocks.size() - 1);
}

void ThreeAddressCode::popBlock()
{
    blockStack.pop();
}

void ThreeAddressCode::addConst(int c)
{
    if ( globaltable.const_num_id.find(c) == std::end(globaltable.const_num_id) )
    {
        auto new_id = globaltable.const_num_id.size();
        globaltable.const_num_id[c] = new_id;
        globaltable.id_to_num[new_id] = c;
    }
}

void ThreeAddressCode::addString(const std::string& str)
{
    if ( globaltable.id_by_string.find(str) == std::end(globaltable.id_by_string) )
    {
        auto new_id = globaltable.id_by_string.size();
        globaltable.id_by_string[str] = new_id;
        globaltable.string_by_id[new_id] = str;
    }
}

void ThreeAddressCode::addExternalFunction(const FunctionSymbol* function)
{
    globaltable.has_definition[function] = false;
}    
    
void ThreeAddressCode::rememberVar(const VariableSymbol* var) 
{
    Block* current_block = blocks[blockStack.top()];
    current_block -> allocate(var);
}
