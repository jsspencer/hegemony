#include <iostream>
#include <string>

#include "Selene/include/selene.h"

#include "info.h"
#include "ueg3d.h"


int main(int argc, char **argv)
{
    print_banner();

    sel::State L(true);
    L["UEG3D"].SetClass<UEG3D, int, double, double, double>(
            "init_basis", &UEG3D::init_basis,
            "MP2", &UEG3D::MP2
            );

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <input file>" << std::endl;
    } else {
        std::string inp = argv[1];
        if (echo_input(inp))
        {
            // RUN!
            L.Load(inp);
        } else {
            std::cout << " File cannot be found: " << inp << std::endl;
        }
    }
    return 0;
}
