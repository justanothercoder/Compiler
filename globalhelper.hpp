#ifndef _GLOBALHELPER_HPP_
#define _GLOBALHELPER_HPP_

#include <string>
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

    static int isAlreadyDefined(Symbol *sym);
    static void setDefined(Symbol *sym);

    static Scope* getASTScope(AST *t);
    static void setASTScope(AST *t, Scope *sc);

	static std::string getCodeOperatorName(std::string op);

private:

    GlobalHelper();

    static map<ExprNode*, Type*> type_hints;
    static map<Symbol*, bool> is_defined;
    static map<AST*, Scope*> ast_scopes;
};

#endif
