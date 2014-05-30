#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "filehelper.hpp"

class ImportNode : public AST
{
public:
	ImportNode(string lib);

    virtual AST* copyTree() const;
	
    virtual void define();
    virtual void check();
    virtual CodeObject& gen();

private:

	string lib;
	CodeObject code_obj;
};

#endif
