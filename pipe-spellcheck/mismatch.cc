#include <iostream>
#include <string>
#include <fstream>
#include <string.h>

int main(int argc, char **argv)
{
    for (std::string line; std::getline(std::cin, line) ;)
    {
		std::ifstream file("/usr/share/dict/words");
		char aWord[50];
		bool breaker = true;
		while (file.good() && (breaker == true))
		{
    		file >> aWord;
    		if (file.good() && strcmp(aWord, line.c_str()) == 0)
    		{
        		breaker = false;
        		//std::cout << "found!" << std::endl;
        		//std::cout << aWord << std::endl;
    		}
    	}
    	if(breaker == true) {std::cout << line << std::endl;}
	}
}