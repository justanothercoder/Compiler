#ifndef _ASSIGNMENTNODE_HPP_
#define _ASSIGNMENTNODE_HPP_

#include "binaryoperatornode.hpp"
#include "overloadedfunctiontypeinfo.hpp"

class AssignmentNode : public BinaryOperatorNode
{
public:

    AssignmentNode(ExprNode *lhs, ExprNode *rhs);

    virtual string getOperatorName();
    virtual void gen();
};

#endif
