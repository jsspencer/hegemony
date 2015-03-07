#pragma once

#include "ueg.h"

class UEG2D : public BaseUEG<2>
{

    public:

    using BaseUEG::BaseUEG;

    double calc_cell(void);

};
