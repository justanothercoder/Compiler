#ifndef _TEMPLATEDECLHOLDER_HPP_
#define _TEMPLATEDECLHOLDER_HPP_

#include <vector>

class AST;

class TemplateDeclHolder
{
public:
    virtual vector<AST*> getChildren() = 0;
};

#endif
