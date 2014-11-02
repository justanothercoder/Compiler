#include "stringnode.hpp"
#include "typefactory.hpp"
#include "builtins.hpp"
#include "globaltable.hpp"

StringNode::StringNode(std::string str) : str(str)
{

}

AST* StringNode::copyTree() const
{
    return new StringNode(str);
}

std::string StringNode::getStr() const
{
    return str;
}

const Type* StringNode::getType() const
{
    static const Type *type = BuiltIns::global_scope -> resolveType("string");

    return TypeFactory::getConst(type);
}

bool StringNode::isLeftValue() const
{
    return false;
}

bool StringNode::isCompileTimeExpr() const
{
    return false;
}

boost::optional<int> StringNode::getCompileTimeValue() const
{
    return boost::none;
}

std::string StringNode::toString() const
{
    return '"' + str + '"';
}

void StringNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}
