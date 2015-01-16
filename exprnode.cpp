#include "exprnode.hpp"

ValueInfo valueOf(ExprNode* expr) { return {expr -> getType(), expr -> isLeftValue()}; }

