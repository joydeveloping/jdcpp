/**
 * \file
 * \brief Cell description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_CELL_H
#define HYDRO_GRID_CELL_H

#include "Solver/Fluid_Dyn_Pars.h"

using namespace std;
using namespace Hydro::Solver;

namespace Hydro { namespace Grid {

/**
 * \brief Cell class.
 */
class Cell
{

public:

    // Two layers of fluid dynamic parameters.
    // Current and Next layers.
    Fluid_Dyn_Pars FDP[2];
};

} }

#endif

