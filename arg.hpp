#ifndef _ARG_HPP_
#define _ARG_HPP_

#include <string>

enum class IdType { NOID, NUMBER, STRING, VARIABLE, TEMP, LABEL, PROCEDURE };

struct Arg
{
	Arg(IdType type, int id);

	std::string toString();

    bool operator==(const Arg& a) const;

	IdType type;
	int id;
};

#endif
