#include "externnode.hpp"

ExternNode::ExternNode(std::string name
                     , std::vector< std::pair<std::string, TypeInfo> > params
                     , TypeInfo return_type_info
                     , bool is_unsafe) : name(name)
                                       , params(params)
                                       , return_type_info(return_type_info)
                                       , is_unsafe(is_unsafe)
{

}

AST* ExternNode::copyTree() const
{
    return new ExternNode(name, params, return_type_info, is_unsafe);
}

std::string ExternNode::toString() const
{
    std::string res = "";
    res += "extern " + name;
    
    res += "(";
    
    if ( !params.empty() )
    {
        auto it = std::begin(params);
        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ")";

    res += " : " + return_type_info.toString();
    return res;
}

void ExternNode::accept(ASTVisitor& visitor)
{
    visitor.visit(this);
}

