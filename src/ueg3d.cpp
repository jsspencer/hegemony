#include "ueg3d.h"

double UEG3D::calc_cell(void)
{
        return rs*pow((4*M_PI*nel)/3,1.0/3);
}


inline double UEG3D::vq(KPoint<3>& q)
{
    // 4\pi / (\Omega q^2)
    int q2 = 0.0;
    for (const int i: q.k) q2 += i*i;
    // Wavevectors are stored in units of 2\pi/L.  We restrict ourselves to the cubic case
    // and hence the integral becomes:
    return 1.0 / (M_PI*L*q2);
}
