#ifndef _FILENOTFOUNDEXCEPTION_HPP_
#define _FILENOTFOUNDEXCEPTION_HPP_

#include <stdexcept>
#include <string>

class FileNotFoundException : public std::logic_error
{
public:

    FileNotFoundException(std::string filename);
    
};

#endif
