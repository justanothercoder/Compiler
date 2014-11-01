#ifndef _FILEHELPER_HPP_
#define _FILEHELPER_HPP_

#include <memory>
#include <fstream>
#include <string>

class AST;

class FileHelper
{
public:

    static AST* parse(std::string filename);

private:

    static std::string extractContents(std::string filename);
};

#endif
