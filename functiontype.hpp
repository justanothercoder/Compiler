#ifndef _FUNCTIONTYPE_HPP_
#define _FUNCTIONTYPE_HPP_

#include <vector>

#include "type.hpp"

using std::vector;

class FunctionType : public Type
{
public:

    FunctionType(Type *return_type, const vector<Type*>& params_types);

    virtual string getName();
    virtual int getSize();    

    Type* getReturnType();
    Type* getParamType(int i);

    int getNumberOfParams();
    
private:

    Type *return_type;
    vector<Type*> params_types;

    int type_size;
};

#endif
