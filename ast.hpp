#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>

class Scope;
struct TemplateInfo;
class CodeObject;

class AST
{
public:

    virtual ~AST();

    virtual AST* copyTree() const = 0;
	
    virtual void build_scope(); 

    virtual void define() = 0;
    virtual void check() = 0;
	virtual CodeObject& gen() = 0;

	virtual std::vector<AST*> getChildren() const;	

	Scope *scope;
};

#endif
