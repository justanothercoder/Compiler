#ifndef _CONSTTYPE_HPP_
#define _CONSTTYPE_HPP_

#include "type.hpp"

class ConstType : public Type
{
public:
	ConstType(Type *const_type);

	Type* getConstType() const;

	virtual string getName() const;
	virtual int getSize() const;

private:
    
    Type *const_type;
    
};

#endif
