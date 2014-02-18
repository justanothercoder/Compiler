#ifndef _GLOBALHELPER_HPP_
#define _GLOBALHELPER_HPP_

#include <map>

class AST;
class ExprNode;
class Type;
class Scope;
class Symbol;

using std::map;

class GlobalHelper
{
public:
    
    static Type* getTypeHint(ExprNode *expr);
    static void setTypeHint(ExprNode *expr, Type *type);

    static int getDefinitionTime(Symbol *sym);
    static int setDefinitionTime(Symbol *sym);

    static Scope* getASTScope(const AST *t);
    static void setASTScope(AST *t, Scope *sc);
    
private:

    GlobalHelper();

    static map<ExprNode*, Type*> type_hints;
    static map<Symbol*, int> definition_time;
    static map<AST*, Scope*> ast_scopes;
};

#endif
