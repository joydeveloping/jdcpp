/**
 * \file
 * \brief Riemann problem solver realization.
 *
 * \author Alexey Rybakov
 */

#include "Riemann.h"

namespace Hydro { namespace Solver {

/**
 * \brief Solve Riemann problem simple by getting average values.
 */
void Riemann::Avg(const Fluid_Dyn_Pars *left_p,
                  const Fluid_Dyn_Pars *right_p,
                  Fluid_Dyn_Pars *center_p)
{
    center_p->Ro = 0.5 * (left_p->Ro + right_p->Ro);
    center_p->V.X = 0.5 * (left_p->V.X + right_p->V.X);
    center_p->V.X = 0.5 * (left_p->V.Y + right_p->V.Y);
    center_p->V.X = 0.5 * (left_p->V.Z + right_p->V.Z);
    center_p->P = 0.5 * (left_p->P + right_p->P);
}

} }

