/**
 * \file
 * \brief Godunov method order of accuracy 1.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_SOLVER_GODUNOV_1_H
#define HYDRO_SOLVER_GODUNOV_1_H

#include "Grid/Grid.h"

using namespace Hydro::Grid;

namespace Hydro { namespace Solver {

/**
 * \brief Godunov method.
 */
class Godunov_1
{

public:

    // Default constructor.
    Godunov_1(Hydro::Grid::Grid *g_p);


    // Iteration.
    void Calc_Iter(double dt);
    void Calc_Iter(Block *b_p,
                   double dt);

private:

    // Grid.
    Hydro::Grid::Grid *G_p_;
};

} }

#endif

