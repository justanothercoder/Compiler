#ifndef _OPTIMIZER_HPP_
#define _OPTIMIZER_HPP_

#include "threeaddresscode.hpp"

class Optimizer
{
public:
    
    Optimizer(ThreeAddressCode& code);

    void optimize();

    void constantPropagation();
    void eliminateUnusedTemporaries();

    ThreeAddressCode& code;
};

#endif
