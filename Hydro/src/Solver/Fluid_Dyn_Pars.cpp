/**
 * \file
 * \brief Fluid Dynamic Parameters realization.
 *
 * \author Alexey Rybakov
 */

#include "Fluid_Dyn_Pars.h"

namespace Hydro { namespace Solver {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars()
    : V(),
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
 * \param[in] ro - density
 * \param[in] p - pressure
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars(double vx,
                               double vy,
                               double vz,
                               double ro,
                               double p)
    : V(vx, vy, vz),
      Ro(ro),
      P(p)
{
}

/**
 * \brief Copy constructor.
 *
 * \param[in] v - speed
 * \param[in] ro - density
 * \param[in] p - pressure
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars(const Vector_3D &v,
                               double ro,
                               double p)
    : V(v),
      Ro(ro),
      P(p)
{
}

} }

