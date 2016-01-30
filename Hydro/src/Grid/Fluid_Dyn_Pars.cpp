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
    V = from_p->V;
    E = from_p->E;
    P = from_p->P;
}

/*
 * Forms: normal <-> expand.
 */

/**
 * \brief Convert normal form to expand.
 */
void Fluid_Dyn_Pars::Normal_To_Expand()
{
    E = R * (E + 0.5 * V.Mod_2());
    V *= R;
}

/**
 * \brief Convert expand form to normal.
 */
void Fluid_Dyn_Pars::Expand_To_Normal()
{
    V /= R;
    E = E / R - 0.5 * V.Mod_2();

    // TODO:
    // Remove.
    P = (1.4 - 1.0) * R * E;
}

} }

