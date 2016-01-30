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
    : R(0.0),
      V(),
      E(0.0),
      P(0.0)
{
}

/**
 * Copy.
 *
 * \param[in] from_p - copy source.
 */
void Fluid_Dyn_Pars::Copy_From(const Fluid_Dyn_Pars *from_p)
{
    R = from_p->R;
    V.X = from_p->V.X;
    V.Y = from_p->V.Y;
    V.Z = from_p->V.Z;
    E = from_p->E;
    P = from_p->P;
}

} }

