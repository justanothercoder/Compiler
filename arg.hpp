#ifndef _ARG_HPP_
#define _ARG_HPP_

#include <string>

class Type;

enum class IdType { NOID, NUMBER, STRING, VARIABLE, TEMP, LABEL, PROCEDURE };

struct Arg
{
	Arg(IdType type, int id, const Type *expr_type = nullptr);

	std::string toString() const;

    bool operator==(const Arg& a) const;

	IdType type;
	int id;

    const Type *expr_type;
};

#endif
