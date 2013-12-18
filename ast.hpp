#ifndef _AST_HPP_
#define _AST_HPP_

#include "scope.hpp"

class AST
{
public:

    virtual ~AST();

    virtual void process() = 0;

    Scope *scope;    
};

#endif
