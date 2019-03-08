#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    std::string a;
    std::cin >> a;
    std::string b;
    std::cin >> b;
    if(a[0] > b[0]) std::cout << "cool" << std::endl;
    std::cout << a << std::endl << b << std::endl;
}