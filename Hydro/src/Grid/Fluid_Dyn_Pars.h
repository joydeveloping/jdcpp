/**
 * \file
 * \brief Fluid Dynamic Parameters.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FLUID_DYN_PARS_H
#define HYDRO_GRID_FLUID_DYN_PARS_H

#include "Lib/Math/Vector_3D.h"

using namespace Lib::Math;

namespace Hydro { namespace Grid {

/**
 * \brief Fluid Dynamic Parameters.
 */
class Fluid_Dyn_Pars
{

public:

    /*
     * Data (not private because class it is just data container).
     */

    // Density.
    double Ro;

    // Speed.
    Vector_3D V;

    // Energy.
    double Eps;

    // Pressure.
    double P;

    /*
     * Functions.
     */

    // Constructors/destructors.
    Fluid_Dyn_Pars();

    // Copy.
    void Copy_From(const Fluid_Dyn_Pars *from_p);
};

} }

#endif

