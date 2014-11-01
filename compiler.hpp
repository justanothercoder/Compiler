#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include "globaltable.hpp"

class DeclarationNode;

class Compiler
{
public:

    const Type* fromTypeInfo(const TypeInfo& type_info, Scope *scope);
    const Type* fromTypeInfo(const TypeInfo& type_info, const TemplateInfo& template_info, Scope *scope);

//    DeclarationNode* getSpecDecl(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params, Scope *scope);
//    const Symbol* getSpec(const TemplateStructSymbol *sym, std::vector<TemplateParam> template_params, Scope *scope);

};

#endif
