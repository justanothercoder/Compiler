#ifndef _FIElDVARALLOCATOR_HPP_
#define _FIElDVARALLOCATOR_HPP_

#include "varallocator.hpp"
#include "globalconfig.hpp"

class FieldVarAllocator : public VarAllocator
{
public:
    FieldVarAllocator();

    virtual void addVariable(VariableSymbol *var);
};

#endif
