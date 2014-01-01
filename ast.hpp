#ifndef _AST_HPP_
#define _AST_HPP_

#include "scope.hpp"

class AST
{
public:

    virtual ~AST();

    virtual void build_scope() = 0;    
    virtual void define() = 0;
    virtual void gen() = 0;

    Scope *scope;    
};

#endif
