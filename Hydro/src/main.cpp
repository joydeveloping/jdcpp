/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "Lib/MPI/mpi.h"
#include "Grid/Grid.h"

/**
 * \brief Name of grid.
 */
#define GRID_NAME "/home1/rybakov/Data/Grids/grid_for_test_50"

using namespace Hydro::Grid;

/**
 * \brief Controlled run.
 *
 * \param[in] ranks_count - count of ranks
 * \param[in] rank - rank number
 */
int Run(int ranks_count,
        int rank)
{
    string ranks_count_string = Lib::IO::To_String(ranks_count);
    while (ranks_count_string.length() < 3)
    {
        ranks_count_string = "0" + ranks_count_string;
    }
    string rank_string = Lib::IO::To_String(rank);
    while (rank_string.length() < 3)
    {
        rank_string = "0" + rank_string;
    }
    string out_name = "out_" + ranks_count_string + "_" + rank_string + ".txt";
    ofstream out(out_name.c_str(), ios::out);

    // General action.
    Grid *grid_p = new Grid();
    grid_p->Load_GEOM(GRID_NAME, ranks_count);
    //out << grid_p;
    grid_p->Calculate_Iterations(100);
    grid_p->Print_Timers(out);
    grid_p->Print_Statistics(out);
    grid_p->Print_Blocks_Distribution(out, ranks_count);
    delete grid_p;
    out.close();
}

/**
 * \brief Main function (enter point).
 *
 * \return
 * Run status.
 */
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    Run(Lib::MPI::Ranks_Count(), Lib::MPI::Rank());
    MPI_Finalize();

    return 0;
}

