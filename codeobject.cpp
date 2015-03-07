#include "codeobject.hpp"
#include "globalconfig.hpp"
#include "comp.hpp"
#include "logger.hpp"

void CodeObject::gen() const { std::cout << code; }

std::string CodeObject::getCode() const { return code; }
void CodeObject::emit(std::string text) { code += text + '\n'; }
void CodeObject::comment(std::string text) { if ( *Comp::config().flagValue("fcommentasm") ) emit("; " + text); }
