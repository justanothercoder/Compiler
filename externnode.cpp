#include "externnode.hpp"

ExternNode::ExternNode(std::string name, FunctionDeclarationInfo info, bool is_unsafe) : name         (name)
                                                                                       , info         (info)
                                                                                       , is_unsafe    (is_unsafe)
                                                                                       , definedSymbol(nullptr)
{

}

AST* ExternNode::copyTree() const { return new ExternNode(name, info, is_unsafe); }

std::string ExternNode::toString() const
{
    std::string res = "";
    res += "extern " + name;
    
    res += "(";
    
    if ( !info.formalParams().empty() )
    {
        const auto& params = info.formalParams();

        auto it = std::begin(params);
        res += it -> second.toString() + " " + it -> first;

        for ( ++it; it != std::end(params); ++it )
            res += ", " + it -> second.toString() + " " + it -> first;
    }

    res += ")";

    res += " : " + info.returnTypeInfo().toString();
    return res;
}

void ExternNode::accept(ASTVisitor& visitor) { visitor.visit(this); }
