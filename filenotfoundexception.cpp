#include "filenotfoundexception.hpp"

FileNotFoundException::FileNotFoundException(std::string filename) : std::logic_error("File not found: '" + filename + "'.")
{

}
