/**
 * \file
 * \brief Cell description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_CELL_H
#define HYDRO_GRID_CELL_H

#include "Lib/Math/Point_3D.h"
#include "Fluid_Dyn_Pars.h"
#include "Direction.h"

using namespace std;
using namespace Lib::Math;

namespace Hydro { namespace Grid {

/**
 * \brief Cell class.
 */
class Cell
{

public:

    // Center.
    Point_3D Center;

    // Volume.
    double Vo;

    // Edges squares.
    double S[Direction::Count];

    // Two layers of fluid dynamic parameters.
    // Current and Next layers.
    Fluid_Dyn_Pars FDP[2];

    // Setters.
    void Set_Center(double x,
                    double y,
                    double z);
    void Set_Descartes_Edges_Squares(double si,
                                     double sj,
                                     double sk);
    void Set_FDP(double vx,
                 double vy,
                 double vz,
                 double ro,
                 double p);
};

} }

#endif

