#ifndef _TYPEMODIFIER_HPP_
#define _TYPEMODIFIER_HPP_

#include <boost/variant.hpp>
#include "exprnode.hpp"

class TypeModifier
{
public:

    TypeModifier();
    TypeModifier(std::shared_ptr<ExprNode> dimension);

    bool isPointer() const;
    bool isDimension() const;

    std::string toString() const;
    boost::optional<ExprNode*> dimension() const;
   
private:
    
    std::shared_ptr<ExprNode> dimension_;
};

bool operator==(const TypeModifier& lhs, const TypeModifier& rhs);

#endif
