#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "filehelper.hpp"

class ImportNode : public AST
{
public:
	ImportNode(string lib);

    virtual AST* copyTree() const;
	
    virtual void define(const TemplateInfo& template_info);
    virtual void check(const TemplateInfo& template_info);
    virtual CodeObject& gen(const TemplateInfo& template_info);

	virtual std::vector<AST*> getChildren() const;

private:

	string lib;
	CodeObject code_obj;
};

#endif
