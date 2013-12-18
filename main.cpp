#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string filename = "input.txt";
    
    std::ifstream in(filename.c_str());

    std::string buf = "";

    char c;
    while ( (c = in.get()) != -1 )
	buf += c;    
    
    in.close();
    
    return 0;
}
