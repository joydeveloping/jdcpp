/**
 * \file
 * \brief Riemann problem solver.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_SOLVER_RIEMANN_H
#define HYDRO_SOLVER_RIEMANN_H

#include "Grid/Fluid_Dyn_Pars.h"

using namespace Hydro::Grid;

namespace Hydro { namespace Solver {

/**
 * \brief Riemann problem solver.
 */
class Riemann
{

public:

    // Simple calculate by average values.
    static void Avg(const Fluid_Dyn_Pars *left_p,
                    const Fluid_Dyn_Pars *right_p,
                    Fluid_Dyn_Pars *center_p);

private:

};

} }

#endif

