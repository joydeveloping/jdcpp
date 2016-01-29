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

    // Speed.
    Vector_3D V;

    // Temperature.
    double T;

    // Density.
    double Ro;

    // Pressure.
    double P;

    /*
     * Functions.
     */

    // Constructors/destructors.
    Fluid_Dyn_Pars();
    Fluid_Dyn_Pars(double vx,
                   double vy,
                   double vz,
                   double t,
                   double ro,
                   double p);
    Fluid_Dyn_Pars(const Vector_3D &v,
                   double t,
                   double ro,
                   double p);
};

} }

#endif

