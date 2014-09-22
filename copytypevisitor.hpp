#ifndef _COPYTYPEVISITOR_HPP_
#define _COPYTYPEVISITOR_HPP_

#include "typevisitor.hpp"
#include "codeobject.hpp"

class CopyTypeVisitor : public TypeVisitor
{
public:

	CopyTypeVisitor(std::string from, std::string to);

	void visit(const ConstType *tp) override;
	void visit(const PointerType *tp) override;
	void visit(const StructSymbol *tp) override;
	void visit(const ReferenceType *tp) override;
	void visit(const FunctionSymbol *tp) override;
	void visit(const BuiltInTypeSymbol *tp) override;
	void visit(const OverloadedFunctionSymbol *tp) override;

	CodeObject getCopyCode(const Type *type);

private:
	std::string from;
	std::string to;

	CodeObject code_obj;
};

#endif
