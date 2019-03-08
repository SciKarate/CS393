#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	std::string alpha = "qwertyuiopasdfghklzxcvbnm";
    for (std::string line; std::getline(std::cin, line) ;)
    {
		for(int i = 0; i < line.length(); i++)
		{
			if(std::string(1, line[i]) == " ") {std::cout << std::endl;}
			else if(isalpha(line[i])) {std::cout << line[i];}
		}
		std::cout << std::endl;
	}
}