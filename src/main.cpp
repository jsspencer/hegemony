#include <iostream>
#include <string>

#include "Selene/include/selene.h"

#include "info.h"
#include "ueg3d.h"

int main(int argc, char **argv)
{
    print_banner();

    sel::State L(true);
    // See:
    // http://stackoverflow.com/questions/22087183/template-deduction-from-pointer-to-base-class-member
    // http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3772.pdf
    L["UEG3D"].SetClass<UEG3D, int, double, double, double>(
            "init_basis", static_cast<void (UEG3D::*)()>(&UEG3D::init_basis),
            "MP2", static_cast<double (UEG3D::*)()>(&UEG3D::MP2),
            "L", static_cast<double UEG3D::*>(&UEG3D::L)
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
