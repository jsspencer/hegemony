#pragma once

#include "ueg.h"

class UEG3D : public BaseUEG<3>
{

    public:

    using BaseUEG::BaseUEG;

    double calc_cell(void);
    inline double vq(KPoint<3>&);

};

