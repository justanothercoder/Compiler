#ifndef _AST_HPP_
#define _AST_HPP_

class AST
{
public:

    virtual ~AST();

    virtual void process() = 0;
    
protected:
    
    Scope *scope;    
};

#endif
