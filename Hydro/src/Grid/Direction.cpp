/**
 * \file
 * \brief Direction functions realization.
 *
 * \author Alexey Rybakov
 */

#include "Direction.h"

namespace Hydro { namespace Grid {

/**
 * \brief Name of direction.
 *
 * \param[in] direction - direction value
 *
 * \return
 * Name of direction
 */
string Direction::Name(int direction)
{
    switch (direction)
    {
        case I0:
            return "IO";

        case I1:
            return "I1";

        case J0:
            return "J0";

        case J1:
            return "J1";

        case K0:
            return "K0";

        case K1:
            return "K1";

        default:
            assert(false);
    }
}

} }

