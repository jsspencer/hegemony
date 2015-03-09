#pragma once

#include <array>

template<int ndim>
class KPoint
{
    public:
    std::array<int,ndim> k;
    double kin = -1;

    KPoint() {} ;

    KPoint(std::initializer_list<int> kin) : k(kin)
    {
        set_kinetic();
    }

    inline void set_kinetic(void)
    {
        kin = 0;
        // NOTE: kinetic energy is in scaled units (ie (2\pi/L)^2). 
        for (const int i : k)
            kin += double(i*i)/2;
    }

    friend inline KPoint operator+(const KPoint &k1, const KPoint &k2)
    {
        KPoint ksum = KPoint();
        for (int i=0; i<ndim; i++)
            ksum.k[i] = k1.k[i] + k2.k[i];
        return ksum;
    }

    friend inline KPoint operator-(const KPoint &k1, const KPoint &k2)
    {
        KPoint kdiff = KPoint();
        for (int i=0; i<ndim; i++)
            kdiff.k[i] = k1.k[i] - k2.k[i];
        return kdiff;
    }

    friend inline bool operator<(const KPoint &lhs, const KPoint &rhs)
    {
        return lhs.kin < rhs.kin;
    }

    friend inline bool operator==(const KPoint &lhs, const KPoint &rhs)
    {
        return lhs.k == rhs.k;
    }

};

namespace std {
    template <int ndim>
    struct hash<KPoint<ndim>>
    {
        std::size_t operator()(const KPoint<ndim>& key) const
        {
            std::size_t offset = 4096;
            std::size_t res = key.k[0];
            for (int i=1; i<ndim; i++)
                res ^= (key.k[i] +offset) << 8*i;
            return res;
        }
    };
}
