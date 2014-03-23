#include "filenotfoundexception.hpp"

FileNotFoundException::FileNotFoundException(string filename) : filename(filename) { }

FileNotFoundException::~FileNotFoundException() noexcept { }

const char* FileNotFoundException::what() const noexcept { return ("File not found: '" + filename + "'.").c_str(); }
