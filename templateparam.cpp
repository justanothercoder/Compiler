#include "templateparam.hpp"

unsigned long long hashTemplateParams(std::vector<TemplateParam> template_params)
{
    unsigned long long P = 31, pow = 1, ans = 0;

    for ( size_t i = 0; i < template_params.size(); ++i )
    {
        if ( template_params[i].which() == 0 )
            ans += static_cast<int>(std::hash<std::string>()(boost::get<TypeInfo>(template_params[i]).name()) * pow);
        else
            ans += static_cast<int>(boost::get<int>(template_params[i])) * pow;

        pow *= P;
    }

    return ans;
}
    
