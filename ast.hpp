#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>
#include <string>

#include "astvisitor.hpp"

class Scope;
struct TemplateInfo;
class CodeObject;

class AST
{
public:

    virtual ~AST();
    virtual AST* copyTree() const = 0;
    virtual void build_scope(); 
	virtual std::vector<AST*> getChildren() const;	
	virtual std::string toString() const = 0;
	virtual void accept(ASTVisitor& visitor) = 0;

	Scope *scope;
};

#endif
