#ifndef _FILEHELPER_HPP_
#define _FILEHELPER_HPP_

#include <memory>
#include <fstream>
#include <string>

#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

using namespace std;

class FileHelper
{
public:

	static AST* parse(string filename);

private:

	static string extractContents(string filename);
};

#endif
