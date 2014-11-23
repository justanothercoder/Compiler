#include "threeaddresscode.hpp"
#include "scope.hpp"
#include "builtins.hpp"
#include "functionsymbol.hpp"
#include "globaltable.hpp"
#include "typefactory.hpp"
#include "pointertype.hpp"

#include "logger.hpp"

Arg ThreeAddressCode::newLabel(std::string label)
{
    static int label_num = 0;

    ++label_num;

    if ( label == "" )
        globaltable.label_name[label_num] = "label_" + std::to_string(label_num);
    else
        globaltable.label_name[label_num] = label;

    return Arg(IdType::LABEL, label_num);
}

Arg ThreeAddressCode::add(Command command)
{
    Block& current_block = blocks[blockStack.top()];

    const Type *command_type;
    
    current_block.commands.push_back(command);
    current_block.code.push_back(current_block.commands.size() - 1);

    switch ( command.op )
    {
    case SSAOp::ASSIGN: case SSAOp::PARAM: case SSAOp::LABEL: case SSAOp::RETURN:
    case SSAOp::IF: case SSAOp::IFFALSE: case SSAOp::GOTO: case SSAOp::RETURNREF:
    case SSAOp::ASSIGNCHAR:
        return Arg(IdType::NOID, -1);
    case SSAOp::PLUS: case SSAOp::MINUS: case SSAOp::MUL:
    case SSAOp::DIV: case SSAOp::MOD: case SSAOp::EQUALS:
    case SSAOp::NEQUALS: case SSAOp::ELEM:
        command_type = BuiltIns::int_type;
        break;
    case SSAOp::DEREF: command_type = static_cast<const PointerType*>(command.arg1.expr_type); break;
    case SSAOp::ADDR : command_type = TypeFactory::getPointer(command.arg1.expr_type); break;
    case SSAOp::DOT  : command_type = globaltable.var_by_id[command.arg2.id] -> getType(); break;
    case SSAOp::CALL : command_type = globaltable.func_by_id[command.arg1.id] -> getType() -> getReturnType(); break;
    case SSAOp::NEW  : command_type = globaltable.type_by_id[command.arg1.id]; break;
    default:
        throw std::logic_error("Not all SSAOp's handled in ThreeAddressCode::add.");
    }

    return Arg(IdType::TEMP, current_block.commands.size() - 1, command_type);
}


std::string ThreeAddressCode::toString()
{
    std::string res;
    for ( auto block : blocks )
        res += block.toString() + "\n";

    return res;
}

void ThreeAddressCode::genAsm(CodeObject& code_obj) const
{
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

        std::string res = "0";
        const auto& str = p.second;
        for ( int i = str.length() - 1; i >= 0; --i )
        {
            if ( i >= 1 && str[i - 1] == '\\' && str[i] == 'n' )
            {
                res.append(", 10");
                --i;
            }
            else
            {
                res.append(", " + std::to_string(static_cast<int>(str[i])));
            }
        }

        code_obj.emit("@" + str_label + ": db " + res);
        code_obj.emit(str_label + ": equ $ - 1");
    }

    code_obj.emit("section .text");

    auto block = blocks.cbegin();

    for ( ; block + 1 != blocks.cend(); ++block )
    {
        if ( !(dynamic_cast<FunctionScope*>(&block -> scope) && dynamic_cast<FunctionScope*>(&block -> scope) -> func -> is_used) )
            continue;

        block -> genAsm(code_obj);
        code_obj.emit("ret");
    }
    
    code_obj.emit("global _start");
    code_obj.emit("_start:");

    block -> genAsm(code_obj);    

    code_obj.emit("mov rax, 60");
    code_obj.emit("mov rdi, 0");
    code_obj.emit("syscall");
}

void ThreeAddressCode::newBlock(Scope& scope, std::string block_name)
{
    if ( block_name == "" )
        block_name = scope.getScopeName();

    blocks.push_back(Block(scope, globaltable, block_name));
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

void ThreeAddressCode::addFunction(const FunctionSymbol *sym)
{
    if ( globaltable.id_by_func.find(sym) == std::end(globaltable.id_by_func) )
    {
        auto new_id = globaltable.id_by_func.size();
        globaltable.id_by_func[sym] = new_id;
        globaltable.func_by_id[new_id] = sym;
    }
}

void ThreeAddressCode::addVariable(VariableSymbol *sym)
{
    if ( globaltable.id_by_var.find(sym) == std::end(globaltable.id_by_var) )
    {
        auto new_id = globaltable.id_by_var.size();
        globaltable.id_by_var[sym] = new_id;
        globaltable.var_by_id[new_id] = sym;
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

int ThreeAddressCode::getConstId(int c)
{
    return globaltable.const_num_id.at(c);
}

int ThreeAddressCode::getFuncId(const FunctionSymbol *sym)
{
    return globaltable.id_by_func.at(sym);
}

int ThreeAddressCode::getVarId(VariableSymbol *sym)
{
    return globaltable.id_by_var.at(sym);
}

int ThreeAddressCode::getStrId(const std::string& str)
{
    return globaltable.id_by_string.at(str);
}

int ThreeAddressCode::getConstFromId(int id)
{
    return globaltable.id_to_num[id];
}

void ThreeAddressCode::addParamInfo(ConversionInfo info)
{
    if ( globaltable.id_by_info.find(info) == std::end(globaltable.id_by_info) )
    {
        auto new_id = globaltable.id_by_info.size();
        globaltable.id_by_info[info] = new_id;
        globaltable.info_by_id.insert({new_id, info});
    }
}

int ThreeAddressCode::getInfoId(ConversionInfo info)
{
    return globaltable.id_by_info.at(info);
}

ConversionInfo ThreeAddressCode::getInfoFromId(int id)
{
    return globaltable.info_by_id.at(id);
}
    
void ThreeAddressCode::addType(const Type *type)
{
    if ( globaltable.id_by_type.find(type) == std::end(globaltable.id_by_type) )
    {
        auto new_id = globaltable.id_by_type.size();
        globaltable.id_by_type[type] = new_id;
        globaltable.type_by_id.insert({new_id, type});
    }
}

int ThreeAddressCode::getTypeId(const Type *type)
{
    return globaltable.id_by_type.at(type);
}

const Type* ThreeAddressCode::getTypeFromId(int id)
{
    return globaltable.type_by_id.at(id);
}

