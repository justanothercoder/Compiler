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
    
	static std::string getCodeOperatorName(std::string op);

	static int transformAddress(Scope *scope, int addr); 

private:

    GlobalHelper();
};

#endif
