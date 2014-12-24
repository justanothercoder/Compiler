#ifndef _COMP_HPP_
#define _COMP_HPP_

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "threeaddresscode.hpp"

class GlobalConfig;
class CompilableUnit;

class Comp
{
public:

    static GlobalConfig& config();

    static CompilableUnit& compile(std::string filename);
    static boost::optional<CompilableUnit&> getUnit(std::string module_name);

    static std::vector<CompilableUnit> units;
    static ThreeAddressCode code;
};

#endif
