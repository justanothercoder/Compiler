#ifndef _GLOBALHELPER_HPP_
#define _GLOBALHELPER_HPP_

#include <map>

#include "exprnode.hpp"

using std::map;

class GlobalHelper
{
public:

    static Type* getTypeHint(ExprNode *expr);
    static void setTypeHint(ExprNode *expr, Type *type);

private:

    GlobalHelper();

    static map<ExprNode*, Type*> type_hints;
};

#endif
