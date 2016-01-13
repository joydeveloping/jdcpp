/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "Grid/Grid.h"
#include "Lib/MPI/mpi.h"

using namespace Hydro::Grid;

/**
 * \brief Main function (enter point).
 *
 * \return
 * Run status.
 */
int main(int argc, char **argv)
{
    // Prepare MPI and output.
    MPI_Init(&argc, &argv);
    string out_name = "out_" + Lib::IO::To_String(Lib::MPI::Rank()) + ".txt";
    ofstream out(out_name.c_str(), ios::out);

    // General action.
    Grid *grid_p = new Grid();
    grid_p->Load_GEOM("../../../ciam/lazurit/Hydro/in/soplo2", Lib::MPI::Ranks_Count());
    out << grid_p;
    grid_p->Calculate_Iterations(10);
    grid_p->Print_Timers(out);
    grid_p->Print_Statistics(out);
    grid_p->Print_Blocks_Distribution(out);
    delete grid_p;

    // Finalize MPI and output.
    out.close();
    MPI_Finalize();

    return 0;
}

