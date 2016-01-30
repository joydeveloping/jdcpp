/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "Lib/MPI/mpi.h"
#include "Grid/Grid.h"
#include "Solver/Godunov_1.h"

/**
 * \brief Name of grid.
 */
#define GRID_NAME "/home1/rybakov/Data/Grids/grid_for_test_50"

using namespace Hydro::Grid;
using namespace Hydro::Solver;

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
 * \brief Run solid descartes test.
 */
int Run_Solid_Descartes()
{
    Grid *grid_p = new Grid();
    Godunov_1 *calculation_p = new Godunov_1(grid_p);

    grid_p->Create_Solid_Descartes(10, 1, 1, 1.0, 1.0, 1.0);
    calculation_p->Calc_Iter(0.001);

    // Print out.
    grid_p->Print_Statistics();

    // Print block info.
    {
        Block *b_p = grid_p->Get_Block(0);
        int lay = grid_p->Layer();

        cout << "------------------------------------------------------------------------"
             << "------------------------------------------" << endl;

        cout << "Ro :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].Ro;
        }
        cout << endl;

        cout << "Vx :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.X;
        }
        cout << endl;

        cout << "Vy :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.Y;
        }
        cout << endl;

        cout << "Vz :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.Z;
        }
        cout << endl;

        cout << "E  :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].Eps;
        }
        cout << endl;

        cout << "P  :";
        for (int i = 0; i < b_p->Cells_Count(); i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].P;
        }
        cout << endl;

        cout << "------------------------------------------------------------------------"
             << "------------------------------------------" << endl;
    }

    delete grid_p;
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
    Run_Solid_Descartes();
    MPI_Finalize();

    return 0;
}

