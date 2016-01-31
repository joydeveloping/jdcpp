/**
 * \file
 * \brief Fluid Dynamic Parameters.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FLUID_DYN_PARS_H
#define HYDRO_GRID_FLUID_DYN_PARS_H

#include "Lib/Math/Vector_3D.h"
#include "Lib/IO/io.h"

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
    double DR_X() const { return R * V.X; }
    double DR_Y() const { return R * V.Y; }
    double DR_Z() const { return R * V.Y; }
    double DV_X() const { return R * V.X * V.X + P; }
    double DV_Y() const { return R * V.Y * V.Y + P; }
    double DV_Z() const { return R * V.Z * V.Z + P; }
    double DE_X() const { return (E + P) * V.X; }
    double DE_Y() const { return (E + P) * V.Y; }
    double DE_Z() const { return (E + P) * V.Z; }
    void Flow_X(double dr,
                double dv,
                double de);
    void Flow_Y(double dr,
                double dv,
                double de);
    void Flow_Z(double dr,
                double dv,
                double de);
    void Flow_X(Fluid_Dyn_Pars &fdp,
                double dr,
                double dv,
                double de);
    void Flow_Y(Fluid_Dyn_Pars &fdp,
                double dr,
                double dv,
                double de);
    void Flow_Z(Fluid_Dyn_Pars &fdp,
                double dr,
                double dv,
                double de);
};

// Print information.
ostream &operator<<(ostream &os,
                    const Fluid_Dyn_Pars *p);

} }

#endif

