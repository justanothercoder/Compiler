#ifndef _COMP_HPP_
#define _COMP_HPP_

#include <string>
#include <vector>
#include "threeaddresscode.hpp"

class CompilableUnit;

class Comp
{
public:

    static CompilableUnit& compile(std::string filename);

    static std::vector<CompilableUnit> units;
    static ThreeAddressCode code;
};

#endif
