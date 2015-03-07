#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cfloat>
#include <vector>

#include <iostream>

#include "basis.h"

template<int ndim>
class BaseUEG
{
    public:
    int nel;
    double rs;
    double cutoff;
    double madelung;

    double L;
    std::vector<KPoint<ndim>> basis;
    std::vector<double> hf;

    BaseUEG(int nel, double rs, double cutoff, double madelung)
        : nel(nel), rs(rs), cutoff(cutoff), madelung(madelung) { };

    virtual double calc_cell(void) { return -1; };

    void init_basis(void)
    {
        L = calc_cell();

        // generate basis set within the desired cutoff.
        // Need to consider all wavevectors up to K in each direction, where K^2/2 = cutoff.
        int nmax = round(ceil(pow(2*cutoff,1.0/2)));
        double npts = double(2*nmax+1);
        std::cout << "nmax " << nmax << " " << npts << std::endl;

        KPoint<ndim> ktest;
        // Consider all wavevectors in the smallest ndim-dimensional cube which contains
        // the ndim-dimensional sphere of radius equal to the cutoff.
        for (int i=0; i < round(pow(npts,ndim)); i++)
        {
            float x = i;
            // Can assign a index to wavevector using index = \sum_i npts^{i-1} k_i, and
            // hence decode the index, where k_i is the component along the i-th axis in
            // units of grid spacings (i.e. (2\pi/Li)^2) with the origin defined at the
            // corner (rather than centre) of the cube.
            for (int j=ndim-1; j>=0; j--)
            {
                ktest.k[j] = int(x / pow(npts,j));
                x = x - ktest.k[j] * pow(npts,j);
                ktest.k[j] -= nmax;
            }
            ktest.set_kinetic();
            if (ktest.kin <= cutoff)
            {
                // Only include basis vectors inside the cutoff sphere.
                basis.push_back(KPoint<ndim>(ktest));
            }
        }

        std::sort(basis.begin(), basis.end());

    }

};
