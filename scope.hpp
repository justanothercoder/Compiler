#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <string>
#include <set>

#include "typeinfo.hpp"

class Type;
class Scope;
class Symbol;
class ScopeVisitor;
class VarAllocator;
class TempAllocator;
class VariableSymbol;
class FunctionSymbol;
struct TemplateInfo;

class Scope
{
public:

    virtual ~Scope();

    virtual Scope* getEnclosingScope() const = 0;
    virtual Symbol* resolve(std::string name) const = 0;

	virtual VarAllocator& getVarAlloc() const = 0;
	virtual TempAllocator& getTempAlloc() const = 0;

    virtual std::string getScopeName() const = 0;

    virtual void accept(ScopeVisitor *visitor) = 0;

	virtual const TemplateInfo& getTemplateInfo() const = 0;
	virtual bool isUnsafeBlock() const;

	void define(Symbol *sym);

	const Type* resolveType(std::string name);
	const Type* fromTypeInfo(TypeInfo type_info);
	const Type* fromTypeInfo(TypeInfo type_info, const TemplateInfo& template_info);
};

#endif
