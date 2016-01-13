/**
 * \file
 * \brief Direction.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_DIRECTION_H
#define HYDRO_GRID_DIRECTION_H

#include <cassert>
#include "Lib/IO/io.h"

namespace Hydro { namespace Grid {

/**
 * \brief Direction.
 */
class Direction
{

public:

    /**
     * \brief Directions enumeration.
     */
    enum
    {
        I0 = 0,   /**< I negative direction */
        I1 = 1,   /**< I positive direction */
        J0 = 2,   /**< J negative direction */
        J1 = 3,   /**< J positive direction */
        K0 = 4,   /**< K negative direction */
        K1 = 5,   /**< K positive direction */
        Count = 6 /**< count of directions */
    };

    // Functions.
    static string Name(int direction);

private:

};

} }

#endif

