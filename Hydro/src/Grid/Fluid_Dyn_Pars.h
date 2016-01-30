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
    double R;

    // Speed.
    // In normal form: speed.
    // In expand form: speed * R
    Vector_3D V;

    // Energy.
    // In noraml form: inner energy per unit
    // In expand form: full energy
    double E;

    // Pressure.
    double P;

    /*
     * Functions.
     */

    // Constructors/destructors.
    Fluid_Dyn_Pars();
    Fluid_Dyn_Pars(const Fluid_Dyn_Pars &fdp);

    // Forms: normal <-> expand.
    void Normal_To_Expand();
    void Expand_To_Normal();

    // Flow.
    void Flow_X(double dr,
                double dv,
                double de);
    void Flow_Y(double dr,
                double dv,
                double de);
    void Flow_Z(double dr,
                double dv,
                double de);
    void Flow_To_X(Fluid_Dyn_Pars &fdp,
                   double dr,
                   double dv,
                   double de);
    void Flow_To_Y(Fluid_Dyn_Pars &fdp,
                   double dr,
                   double dv,
                   double de);
    void Flow_To_Z(Fluid_Dyn_Pars &fdp,
                   double dr,
                   double dv,
                   double de);
};

} }

#endif

