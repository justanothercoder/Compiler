#ifndef _THREEADRESSCODE_HPP_
#define _THREEADRESSCODE_HPP_

#include <vector>
#include <stack>

#include "block.hpp"
#include "codeobject.hpp"

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
 * |    t = a.b       |  a   |  b   |    .    |
 * +------------------+------+------+---------+
 */

class Scope;

class ThreeAddressCode
{
    friend class Optimizer;

public:

	Arg add(Command command);
	Arg newTemp();
	Arg newLabel(std::string label = "");

	std::string toString();

	CodeObject genAsm() const;

    void newBlock(Scope& scope);
    void popBlock();

private:

    std::vector<Block> blocks;
    std::stack<int> blockStack;
};

#endif
