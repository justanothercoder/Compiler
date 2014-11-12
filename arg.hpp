#ifndef _ARG_HPP_
#define _ARG_HPP_

#include <string>

class Type;

enum class IdType
{
    NOID, NUMBER, STRING, VARIABLE, TEMP, LABEL, PROCEDURE
};

struct Arg
{
    Arg(IdType type, int id);
    Arg(IdType type, int id, const Type *expr_type);

//	std::string toString() const;

    bool operator==(const Arg& a) const;

    IdType type;
    int id;

    const Type *expr_type;
};

namespace std
{
template<>
struct hash<Arg>
{
    typedef Arg argument_type;
    typedef std::size_t result_type;

    result_type operator() (const argument_type& s) const
    {
        return (static_cast<unsigned long long>(s.type) << 32) + s.id;
    }
};
}

#endif
