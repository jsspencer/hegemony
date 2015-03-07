#include <iostream>
#include <fstream>

#ifdef _OPENMP
#include <omp.h>
#endif

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

void print_banner(void)
{
    std::cout << " --- Hegemony --- " << std::endl;
    const time_t now = time((time_t*)NULL);
    std::cout << " " << ctime(&now);
    std::cout << " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << " Git SHA1: " << EXPAND(GIT_SHA1) << std::endl;
#ifdef _OPENMP
    if (omp_get_max_threads() > 1)
        std::cout << " Running on " << omp_get_max_threads() << " threads" << std::endl;
#endif
    std::cout << " ---------------- " << std::endl << std::endl;
}

int echo_input(const std::string &inp)
{
    std::ifstream f(inp.c_str());
    if (f.good())
    {
        std::string l;
        std::cout << " Input file" << std::endl;
        std::cout << " ----------" << std::endl << std::endl;
        while (std::getline(f,l))
        {
            std::cout << l << std::endl;
        }
        std::cout << std::endl << " ----------" << std::endl << std::endl;
        f.close();
        return 1;
    } else {
        f.close();
        return 0;
    }
}
