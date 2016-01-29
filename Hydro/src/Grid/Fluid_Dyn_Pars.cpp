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

