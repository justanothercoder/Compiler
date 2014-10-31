#include "codeobject.hpp"
#include "logger.hpp"
#include "functionsymbol.hpp"
#include "callhelper.hpp"
#include "type.hpp"
#include "builtins.hpp"
#include "callinfo.hpp"
#include "exprnode.hpp"

#include "copytypevisitor.hpp"

void CodeObject::gen() const 
{ 
	std::cout << code; 
}

std::string CodeObject::getCode() const 
{ 
	return code; 
}

void CodeObject::emit(std::string text) 
{
   	code += text + '\n'; 
}
