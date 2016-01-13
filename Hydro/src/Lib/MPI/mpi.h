/**
 * \file
 * \brief MPI primitives description.
 *
 * \author Alexey Rybakov
 */

#ifndef LIB_MPI_MPI_H
#define LIB_MPI_MPI_H

#include <mpi.h>
#include "Timer.h"

namespace Lib { namespace MPI {

// Global functions.
int Rank();
int Ranks_Count();

} }

#endif

