#ifndef _GLOBALTABLE_HPP_
#define _GLOBALTABLE_HPP_

#include <map>
#include <string>
#include <unordered_map>

#include "templateparam.hpp"
#include "conversioninfo.hpp"

class AST;
class ExprNode;
class Type;
class Scope;
class Symbol;
class FunctionSymbol;
class VariableSymbol;

class TypeInfo;
class TemplateStructSymbol;
struct TemplateInfo;

class GlobalTable
{
public:

    static int transformAddress(Scope *scope, int addr);

    std::map<FunctionSymbol*, bool> has_definition;

    std::map<int, int> const_num_id;
    std::map<int, int> id_to_num;

    std::map<VariableSymbol*, int> id_by_var;
    std::map<int, VariableSymbol*> var_by_id;

    std::map<int, std::string> label_name;

    std::map<const FunctionSymbol*, int> id_by_func;
    std::map<int, const FunctionSymbol*> func_by_id;

    std::map<std::string, int> id_by_string;
    std::map<int, std::string> string_by_id;

    std::map<ConversionInfo, int> id_by_info;
    std::map<int, ConversionInfo> info_by_id;
    
    std::map<const Type*, int> id_by_type;
    std::map<int, const Type*> type_by_id;
};

#endif
