/**
 * \file
 * \brief Fluid Dynamic Parameters realization.
 *
 * \author Alexey Rybakov
 */

#include "Fluid_Dyn_Pars.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars()
    : V(),
      T(0.0),
      Ro(0.0),
      P(0.0)
{
}

/**
 * \brief Direct constructor.
 *
 * \param[in] vx - speed x component
 * \param[in] vy - speed y component
 * \param[in] vz - speed z component
 * \param[in] t - temperature
 * \param[in] ro - density
 * \param[in] p - pressure
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars(double vx,
                               double vy,
                               double vz,
                               double t,
                               double ro,
                               double p)
    : V(vx, vy, vz),
      T(t),
      Ro(ro),
      P(p)
{
}

/**
 * \brief Copy constructor.
 *
 * \param[in] v - speed
 * \param[in] t - temperature
 * \param[in] ro - density
 * \param[in] p - pressure
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars(const Vector_3D &v,
                               double t,
                               double ro,
                               double p)
    : V(v),
      T(t),
      Ro(ro),
      P(p)
{
}

/**
 * Copy.
 *
 * \param[in] from_p - copy source.
 */
void Fluid_Dyn_Pars::Copy_From(const Fluid_Dyn_Pars *from_p)
{
    V.X = from_p->V.X;
    V.Y = from_p->V.Y;
    V.Z = from_p->V.Z;
    T = from_p->T;
    Ro = from_p->Ro;
    P = from_p->P;
}

} }

