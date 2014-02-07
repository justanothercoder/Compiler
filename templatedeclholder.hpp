#ifndef _TEMPLATEDECLHOLDER_HPP_
#define _TEMPLATEDECLHOLDER_HPP_

#include <vector>

class AST;
class Scope;

class TemplateDeclHolder
{
public:
    virtual vector<AST*> getChildren() const = 0;
    virtual Scope* getDeclScope() const = 0;
};

#endif
