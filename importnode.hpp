#ifndef _IMPORTNODE_HPP_
#define _IMPORTNODE_HPP_

#include "filehelper.hpp"

class ImportNode : public AST
{
public:
	ImportNode(string lib);

    AST* copyTree() const override;
	
    void define() override;
    void check() override;

    CodeObject& gen() override;

	int neededSpaceForTemporaries() override;

private:

	string lib;
	CodeObject code_obj;
};

#endif
