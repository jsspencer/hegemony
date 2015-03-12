#pragma once

#include <algorithm>
#include <cmath>
#include <unordered_map>
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
    std::unordered_map<KPoint<ndim>, size_t> basis_map;

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
            // It is useful to be able to look up the Hartree--Fock energy from a given
            // wavevector.  Use a hash table to store the index of each wavevector...
            basis_map[(*ki)] = ki - basis.begin();
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

    double MP2(void)
    {
        // For closed-shell systems (RHF reference):
        // E_MP2 = \sum_{ij,ab} \frac{2<ij|ab>(<ab|ij> - <ab|ji>}{e_i + e_j - e_a - e_b}
        //       = \sum_{ij,a<b} 2\frac{2<ij|ab>(<ab|ij> - <ab|ji>}{e_i + e_j - e_a - e_b}
        // (Note: due to conservation of momentum and k_i and k_j are both smaller than k_a and k_b.)
        double EMP2 = 0.0;
        KPoint<ndim> q;
        for (auto ki=basis.begin(); ki!=basis.begin()+nel/2; ki++)
        {
            size_t ki_ind = ki - basis.begin();
            for (auto kj=basis.begin(); kj!=basis.begin()+nel/2; kj++)
            {
                size_t kj_ind = kj - basis.begin();
                for (auto ka=basis.begin()+nel/2; ka!=basis.end(); ka++)
                {
                    //kb is now uniquely defined in order to conserve momentum.
                    //kb->first is the KPoint object, kb->second the index in basis/hf vectors.
                    auto kb = basis_map.find((*ki) + (*kj) - (*ka));
                    if (kb != basis_map.end())
                    {
                        size_t ka_ind = ka - basis.begin();
                        //kb exists in the basis, so include the contribution (ki,kj,ka,kb)
                        //in the MP2 energy.  By construction, momentum symmetry is obeyed.
                        if (ka-basis.begin() < kb->second)
                        {
                            q = (*ki)-(*ka);
                            double int1 = vq(q);
                            q = (*kj)-(*ka);
                            double int2 = vq(q);
                            EMP2 += ( int1*(2*int1-int2) ) / ( hf[ki_ind] + hf[kj_ind] - hf[ka_ind] - hf[kb->second] );
                        }
                    }
                }
            }
        }
        // Account for contribution of (b,a).
        return 2*EMP2;
    }

};
