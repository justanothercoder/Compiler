#include "addrnode.hpp"
#include "typefactory.hpp"
#include "type.hpp"
#include "globalconfig.hpp"
#include "semanticerror.hpp"
#include "scope.hpp"
#include "tempallocator.hpp"
#include "globalhelper.hpp"
#include "pointertype.hpp"

AddrNode::AddrNode(ExprNode *expr, AddrOp op) : expr(expr), op(op)
{

}
	
const Type* AddrNode::getType() const
{
	if ( op == AddrOp::REF )
		return TypeFactory::getPointer(expr -> getType());
	else
	{
		auto type = expr -> getType();
		bool is_const = type -> isConst();

		type = static_cast<const PointerType*>(type -> getUnqualifiedType()) -> getPointedType();

		if ( is_const )
			type = TypeFactory::getConst(type);

		return type;
	}
}

bool AddrNode::isLeftValue() const
{
//	return false;
	return (op == AddrOp::DEREF);
}

void AddrNode::freeTempSpace() 
{

}

bool AddrNode::isCompileTimeExpr() const 
{
	return false;
}

boost::optional<int> AddrNode::getCompileTimeValue() const
{
	return boost::none;
}

AST* AddrNode::copyTree() const
{
	return new AddrNode(static_cast<ExprNode*>(expr -> copyTree()), op);
}
	
void AddrNode::check() 
{
	expr -> check();

	if ( op == AddrOp::REF )
	{
		if ( !expr -> isLeftValue() )
			throw SemanticError("expression is not an lvalue");

		scope -> getTempAlloc().add(GlobalConfig::int_size);
	}
	else
	{
		auto type = expr -> getType() -> getUnqualifiedType();

		if ( type -> getTypeKind() != TypeKind::POINTER )
		   throw SemanticError("expression is not a pointer");	
	}
}

CodeObject& AddrNode::gen() 
{
	code_obj.emit(expr -> gen().getCode());

	if ( op == AddrOp::REF )
	{
		auto addr = "[rbp - " + std::to_string(GlobalHelper::transformAddress(scope, scope -> getTempAlloc().getOffset())) + "]";
		scope -> getTempAlloc().claim(GlobalConfig::int_size);

		code_obj.emit("mov " + addr + ", rax");
	   	code_obj.emit("lea rax, " + addr);
	}
	else
	{
		if ( expr -> getType() -> isReference() )
			code_obj.emit("mov rax, [rax]");
		code_obj.emit("mov rax, [rax]");
	}

	return code_obj;
}
	    
std::vector<AST*> AddrNode::getChildren() const
{
	return {expr};
}

std::string AddrNode::toString() const
{
	return (op == AddrOp::REF ? std::string("&") : std::string("*")) + expr -> toString();
}

void AddrNode::accept(ASTVisitor *visitor)
{
	visitor -> visit(this);
}
