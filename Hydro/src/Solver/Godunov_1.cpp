/**
 * \file
 * \brief Godunov Method order of accuracy 1.
 *
 * \author Alexey Rybakov
 */

#include "Godunov_1.h"

namespace Hydro { namespace Solver {

/**
 * \brief Default constructor.
 *
 * \param[in] g_p - grid pointer
 */
Godunov_1::Godunov_1(Hydro::Grid::Grid *g_p)
    : G_p_(g_p)
{
}


/**
 * \brief Iteration calculation.
 *
 * \param[in] dt - time step
 */
void Godunov_1::Calc_Iter(double dt)
{
}

} }

