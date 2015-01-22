#include "typemodifier.hpp"

TypeModifier::TypeModifier() : dimension_(nullptr) { }
TypeModifier::TypeModifier(std::shared_ptr<ExprNode> dimension) : dimension_(dimension) { }

bool TypeModifier::isPointer() const { return dimension_ == nullptr; }
bool TypeModifier::isDimension() const { return dimension_ != nullptr; }

std::string TypeModifier::toString() const { return dimension_ == nullptr ? std::string("*") : "[" + dimension_ -> toString() + "]"; }
    
boost::optional<ExprNode*> TypeModifier::dimension() const 
{
    if ( isPointer() )
        return boost::none;
    else
        return dimension_.get();
}

bool operator==(const TypeModifier& lhs, const TypeModifier& rhs)
{
    if ( lhs.isPointer() )
        return rhs.isPointer();

    if ( rhs.isPointer() )
        return lhs.isPointer();

    return *lhs.dimension() == *rhs.dimension();
}
