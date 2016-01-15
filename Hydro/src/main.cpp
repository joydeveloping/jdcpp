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
 * \brief Controlled run.
 *
 * \param[in] ranks_count - count of ranks
 * \param[in] rank - rank number
 */
int Run(int ranks_count,
        int rank)
{
    string out_name = "out_"
                      + Lib::IO::To_String(ranks_count) + "_"
                      + Lib::IO::To_String(rank) + ".txt";
    ofstream out(out_name.c_str(), ios::out);

    // General action.
    Grid *grid_p = new Grid();
    grid_p->Load_GEOM("../../../Grids/grid_for_test_50", ranks_count);
    out << grid_p;
    //grid_p->Calculate_Iterations(1);
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
    for (int i = 1; i <= 220; i++)
    {
        //Run(Lib::MPI::Ranks_Count(), Lib::MPI::Rank());
        cout << "Run: " << i << endl;
        Run(i, 0);
    }
    MPI_Finalize();

    return 0;
}

