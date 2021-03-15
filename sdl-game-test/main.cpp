#include "game.hpp"
#include <exception>
#include <iostream>

int main(int argc, char const *argv[])
{
    try
    {
        Game g;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
