#ifndef _FILENOTFOUNDEXCEPTION_HPP_
#define _FILENOTFOUNDEXCEPTION_HPP_

#include <exception>
#include <string>

using std::string;
using std::exception;

class FileNotFoundException : public exception
{
public:

    FileNotFoundException(string filename);
    
    virtual ~FileNotFoundException() noexcept;
    virtual const char* what() const noexcept;
    
private:

    string filename;
    
};

#endif
