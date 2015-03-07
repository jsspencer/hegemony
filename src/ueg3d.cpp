#include "ueg3d.h"

double UEG3D::calc_cell(void)
{
        return rs*pow(4*M_PI/(3*nel),1.0/3);
}
