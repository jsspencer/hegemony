#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include <iostream>

#include "basis.h"

// NOTE: we assume restricted (i.e. Ms=0) electron gases throughout.

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
    virtual inline double vq(KPoint<ndim>&) {return -1; } ;

    inline double vijab(KPoint<ndim>& ki, KPoint<ndim>& kj, KPoint<ndim>& ka, KPoint<ndim>& kb)
    {
        // Check angular momentum is conserved.  Note: we assume spin is...
        KPoint<ndim> kdiff = ki + kj - ka - kb;
        for (const int i: kdiff.k)
        {
            if (i!=0) return 0.0;
        }
        kdiff = ki - ka;
        return vq(kdiff);
    }

    void init_basis(void)
    {
        L = calc_cell();

        // generate basis set within the desired cutoff.
        // Need to consider all wavevectors up to K in each direction, where K^2/2 = cutoff.
        int nmax = round(ceil(pow(2*cutoff,1.0/2)));
        double npts = double(2*nmax+1);

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

        // Plane waves are the restricted Hartree--Fock solution.
        // Calculate single-particle Hartree--Fock eigenvalues using the Aufbau
        // principle.
        for (auto ki=basis.begin(); ki!=basis.end(); ki++)
        {
            // wavevectors are stored in units of 2\pi/L.
            double hf_sp = (*ki).kin * pow((2*M_PI)/L,2);
            if (ki<basis.begin()+nel/2) hf_sp -= madelung/L; // for the <ii|ii> term.
            for (auto kj=basis.begin(); kj!=basis.begin()+nel/2; kj++)
            {
                if (ki!=kj)
                {
                    KPoint<ndim> q = (*ki) - (*kj);
                    hf_sp -= vq(q);
                }
            }
            hf.push_back(hf_sp);
        }

    }

};
