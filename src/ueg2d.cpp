#include <cmath>

#include "ueg2d.h"

double UEG2D::calc_cell(void)
{
    return rs*pow(M_PI/nel,1.0/2);
}

inline double UEG2D::vq(KPoint<2>& q)
{
    // 2\pi / (\Omega |q|)
    int q2 = 0.0;
    for (const int i: q.k) q2 += i*i;
    // Wavevectors are stored in units of 2\pi/L.  We restrict ourselves to the square case
    // and hence the integral becomes:
    return 1.0 / (L*pow(q2,1.0/2));
}
