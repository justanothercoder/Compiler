#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>

#include "globalhelper.hpp"

class Scope;
struct TemplateInfo;
class CodeObject;

class AST
{
public:

    virtual ~AST();

    virtual AST* copyTree() const = 0;
	
    virtual void build_scope(); 

    virtual void define(const TemplateInfo& template_info) = 0;
    virtual void check(const TemplateInfo& template_info) = 0;
    virtual CodeObject& gen(const TemplateInfo& template_info) = 0;

	virtual std::vector<AST*> getChildren() const;	

	Scope *scope;
};

#endif
