/**
 * \file Cell realization.
 *
 * \author Alexey Rybakov
 */

#include "Cell.h"

namespace Hydro { namespace Grid {

/**
 * \brief Set center coordinates.
 *
 * \param[in] x - speed x component
 * \param[in] y - speed y component
 * \param[in] z - speed z component
 */
void Cell::Set_Center(double x,
                      double y,
                      double z)
{
    Center.Set(x, y, z);
}

/**
 * \brief Set edges squares for Descartes coordinate system.
 *
 * \param[in] si - square in i direction
 * \param[in] sj - square in j direction
 * \param[in] sk - square in k direction
 */
void Cell::Set_Descartes_Edges_Squares(double si,
                                       double sj,
                                       double sk)
{
    S[Direction::I0] = si;
    S[Direction::I1] = si;
    S[Direction::J0] = sj;
    S[Direction::J1] = sj;
    S[Direction::K0] = sk;
    S[Direction::K1] = sk;
}

/**
 * \brief Set fluid dynamic parameters.
 *
 * \param[in] vx - speed x component
 * \param[in] vy - speed y component
 * \param[in] vz - speed z component
 * \param[in] ro - density
 * \param[in] p - pressure
 */
void Cell::Set_FDP(double vx,
                   double vy,
                   double vz,
                   double ro,
                   double p)
{
    FDP[0].V.Set(vx, vy, vz);
    FDP[0].Ro = ro;
    FDP[0].P = p;
}

} }

