#ifndef _THREEADRESSCODE_HPP_
#define _THREEADRESSCODE_HPP_

#include <vector>
#include <stack>
#include "block.hpp"
#include "globaltable.hpp"

/* list of SSA commands
 *
 * I use three address code (TAC) as low-level IR.
 * It will be implemented with indirect triples.
 *
 * +------------------+------+------+---------+
 * |      command     | arg1 | arg2 |   op    |
 * +------------------+------+------+---------+
 * |    t = a + b     |  a   |  b   |    +    |
 * +------------------+------+------+---------+
 * |    t = a - b     |  a   |  b   |    -    |
 * +------------------+------+------+---------+
 * |    t = a * b     |  a   |  b   |    *    |
 * +------------------+------+------+---------+
 * |    t = a / b     |  a   |  b   |    /    |
 * +------------------+------+------+---------+
 * |    t = a % b     |  a   |  b   |    %    |
 * +------------------+------+------+---------+
 * |    t = a == b    |  a   |  b   |   ==    |
 * +------------------+------+------+---------+
 * |    t = a[i]      |  a   |  i   |   []    |
 * +------------------+------+------+---------+
 * |    t = *a        |  a   |      |   (*)   |
 * +------------------+------+------+---------+
 * |    t = &a        |  a   |      |    &    |
 * +------------------+------+------+---------+
 * |    a = b         |  a   |  b   |    =    |
 * +------------------+------+------+---------+
 * |    param a       |  a   |      |  param  |
 * +------------------+------+------+---------+
 * |    call p n      |  p   |  n   |  call   |
 * +------------------+------+------+---------+
 * |    goto L        |  L   |      |  goto   |
 * +------------------+------+------+---------+
 * |   if x goto L    |  x   |  L   |   if    |
 * +------------------+------+------+---------+
 * | ifFalse x goto L |  x   |  L   | ifFalse |
 * +------------------+------+------+---------+
 * |    t = new T     |  t   |  T   |   new   |
 * +------------------+------+------+---------+
 */

class Scope;
class CodeObject;

class ThreeAddressCode
{
    friend class Optimizer;

public:

    Argument add(std::shared_ptr<Command> command);
    Argument newLabel(const std::string& label = "");

    void rememberVar(const VarSymbol* var);

    std::string toString();

    void computeMemoryDisposition() const;
    void genAsm(CodeObject& code_obj) const;

    void newBlock(Scope* scope, std::string block_name = "");
    void popBlock();

    void addConst(int c);
    void addString(const std::string& str);

    void addExternalFunction(const FunctionalSymbol* sym);

//private:

    std::string getAsmName(const FunctionalSymbol* sym) const;

    std::vector< std::unique_ptr<Block> > blocks;
    std::stack<int> blockStack;

    GlobalTable globaltable;
};

#endif
