/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "mpi.h"
#include "Grid/Grid.h"

using namespace Hydro::Grid;

/**
 * \brief Main function (enter point).
 *
 * \return
 * Run status.
 */
int main(int argc, char **argv)
{
    int ranks_count;

    MPI_Init(&argc, &argv);

    Grid *grid_p = new Grid();

    MPI_Comm_size(MPI_COMM_WORLD, &ranks_count);
    grid_p->Load_GEOM("../../../ciam/lazurit/Hydro/in/soplo2", ranks_count);
    cout << grid_p;

    delete grid_p;

    MPI_Finalize();

    return 0;
}

