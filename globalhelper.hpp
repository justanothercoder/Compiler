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

    static Scope* getSymbolScope(Symbol *sym);
    static void setSymbolScope(Symbol *sym, Scope *scope);

    static int getDefinitionTime(Symbol *sym);

    static Scope* getASTScope(const AST *t);
    static void setASTScope(AST *t, Scope *sc);
    
private:

    GlobalHelper();

    static map<ExprNode*, Type*> type_hints;
    static map<Symbol*, Scope*> symbol_scopes;

    static map<Symbol*, int> definition_time;

    static map<AST*, Scope*> ast_scopes;
};

#endif
