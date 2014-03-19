#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>

#include "globalhelper.hpp"

class Scope;
struct TemplateInfo;

class AST
{
public:

    virtual ~AST();

    Scope* getScope() const;
    void setScope(Scope *sc);
    
    virtual AST* copyTree() const = 0;
	
    virtual void build_scope(); 

    virtual void define(const TemplateInfo& template_info) = 0;
    virtual void check(const TemplateInfo& template_info) = 0;
    virtual void gen(const TemplateInfo& template_info) = 0;

	virtual std::vector<AST*> getChildren() const = 0;
};

#endif
