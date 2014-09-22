#ifndef _TYPEVISITOR_HPP_
#define _TYPEVISITOR_HPP_

class ConstType;
class PointerType;
class StructSymbol;
class ReferenceType;
class FunctionSymbol;
class BuiltInTypeSymbol;
class OverloadedFunctionSymbol;

class TypeVisitor
{
public:
	
	virtual void visit(const ConstType *tp) = 0;
	virtual void visit(const PointerType *tp) = 0;
	virtual void visit(const StructSymbol *tp) = 0;
	virtual void visit(const ReferenceType *tp) = 0;
	virtual void visit(const FunctionSymbol *tp) = 0;
	virtual void visit(const BuiltInTypeSymbol *tp) = 0;
	virtual void visit(const OverloadedFunctionSymbol *tp) = 0;

};

#endif
