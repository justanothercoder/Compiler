#ifndef _THREEADRESSCODE_HPP_
#define _THREEADRESSCODE_HPP_

#include <vector>
#include <stack>

#include "block.hpp"
#include "codeobject.hpp"

#include "globaltable.hpp"
#include "conversioninfo.hpp"

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

class ThreeAddressCode
{
    friend class Optimizer;

public:

    Arg* add(Command* command);
    Arg* newLabel(std::string label = "");

    std::string toString();

    void genAsm(CodeObject& code_obj) const;

    void newBlock(Scope& scope, std::string block_name = "");
    void popBlock();

    void addConst(int c);
    void addString(const std::string& str);

    void addExternalFunction(const FunctionSymbol* sym);

    int getConstId(int c);
    int getStrId(const std::string& str);

    int getConstFromId(int id);

//private:

    std::vector<Block*> blocks;
    std::stack<int> blockStack;

    GlobalTable globaltable;
};

#endif
