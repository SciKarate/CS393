#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char **argv) {
    for (std::string line; std::getline(std::cin, line) ;)
    {
    	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::cout << line;
        std::cout << "\n";
    }
}