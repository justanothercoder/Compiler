#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include <memory>
#include <vector>
#include "templateparam.hpp"
#include "variabletype.hpp"

class Scope;
class Symbol;
class TypeInfo;
class TemplateSymbol;
class DeclarationNode;

class Compiler
{
public:   

    VariableType fromTypeInfo(const TypeInfo& type_info, Scope* scope);

    std::shared_ptr<DeclarationNode> getSpecDecl(const TemplateSymbol* sym, std::vector<TemplateParam> template_params);
    const Symbol* getSpec(const TemplateSymbol* sym, std::vector<TemplateParam> template_params);

};

#endif
