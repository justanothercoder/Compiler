#include "codeobject.hpp"
#include "logger.hpp"

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
