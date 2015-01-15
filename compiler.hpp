#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include "globaltable.hpp"
#include "variabletype.hpp"

class DeclarationNode;
class DefineVisitor;
class Symbol;
class TemplateSymbol;

class Compiler
{
public:   

    VariableType fromTypeInfo(const TypeInfo& type_info, Scope *scope);

    std::shared_ptr<DeclarationNode> getSpecDecl(const TemplateSymbol* sym, std::vector<TemplateParam> template_params);
    const Symbol* getSpec(const TemplateSymbol* sym, std::vector<TemplateParam> template_params);

};

#endif
