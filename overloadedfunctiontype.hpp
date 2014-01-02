#ifndef _OVERLOADEDFUNCTIONTYPE_HPP_
#define _OVERLOADEDFUNCTIONTYPE_HPP_

#include <set>
#include <map>

#include "functiontype.hpp"

using std::set;
using std::map;

class FunctionSymbol;

class OverloadedFunctionType : public Type 
{
public:

    virtual string getName();
    virtual int getSize();
    
    OverloadedFunctionType(const set<FunctionType*>& overloads);

    set<FunctionType*> overloads;
    map<FunctionType*, FunctionSymbol*> symbols;
};

#endif
