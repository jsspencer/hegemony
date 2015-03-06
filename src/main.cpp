#include <iostream>
#include <string>

#include "info.h"

int main(int argc, char **argv)
{
    print_banner();

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <input file>" << std::endl;
    } else {
        std::string inp = argv[1];
        if (echo_input(inp))
        {
            // RUN!
        } else {
            std::cout << " File cannot be found: " << inp << std::endl;
        }
    }

    return 0;
}
