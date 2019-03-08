#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	std::string prevline = "";
    for (std::string line; std::getline(std::cin, line) ;)
    {
    	if(prevline != line)
    	{
    		std::cout << line << std::endl;
    		prevline = line;
    	}
    }
}