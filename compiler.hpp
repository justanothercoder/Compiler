#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include "globaltable.hpp"

class DeclarationNode;

class DefineVisitor;

class Compiler
{
public:   

    const Type* fromTypeInfo(const TypeInfo& type_info, Scope *scope);

    DeclarationNode* getSpecDecl(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params);
    const Symbol* getSpec(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params);

};

#endif
