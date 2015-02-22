#ifndef _BUILTINS_HPP_
#define _BUILTINS_HPP_

#include <memory>

class Type;
class Scope;

class BuiltIns
{
public:

    static void defineBuiltIns();

    static std::shared_ptr<Scope> global_scope;
    static const Type* void_type;
    static const Type* int_type;
    static const Type* char_type;
    static const Type* ASCII_string_type;    
};
    
bool isIntType(const Type* t);
bool isCharType(const Type* t);
bool isPointer(const Type* t);
bool isReference(const Type* t);
bool isSimpleType(const Type* t);

#endif
