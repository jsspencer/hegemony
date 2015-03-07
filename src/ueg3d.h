#pragma once

#include "ueg.h"

class UEG3D : public BaseUEG<3>
{

    public:

    using BaseUEG::BaseUEG;

    double calc_cell(void);

    // Simple wrappers.
    // selene (sadly) has problems binding to an inherited method, so wrap the base
    // methods we wish to expose here.
    void init_basis(void) { BaseUEG::init_basis(); }

};

