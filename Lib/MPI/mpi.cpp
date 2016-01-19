/**
 * \file
 * \brief MPI primitives realization.
 *
 * \author Alexey Rybakov
 */

#include "mpi.h"

namespace Lib { namespace MPI {

/**
 * \brief Get MPI rank.
 *
 * \return
 * MPI rank.
 */
int Rank()
{
    static int r = 0;
    static int is_r = false;

    if (!is_r)
    {
        MPI_Comm_rank(MPI_COMM_WORLD, &r);
        is_r = true;
    }

    return r;
}

/**
 * \brief Get MPI ranks count.
 *
 * \return
 * MPI ranks count.
 */
int Ranks_Count()
{
    static int rc = 0;
    static int is_rc = false;

    if (!is_rc)
    {
        MPI_Comm_size(MPI_COMM_WORLD, &rc);
        is_rc = true;
    }

    return rc;
}

} }

