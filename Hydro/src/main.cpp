/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "Lib/MPI/mpi.h"
#include "Lib/OMP/omp.h"
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
    cout << "Run_Solid_Descartes : max threads = " << omp_get_max_threads() << endl;
    Grid *grid_p = new Grid();
    Godunov_1 *calculation_p = new Godunov_1(grid_p);

    grid_p->Create_Solid_Descartes(1000, 1000, 1, 1.0, 1.0, 1.0);
    Lib::OMP::Timer *t_p = new Lib::OMP::Timer();
    t_p->Start();
    calculation_p->Calc_Iters(1, 0.00001);
    t_p->Stop();
    cout << "Time : " << t_p->Time() << endl;
    delete t_p;

    // Print out.
    grid_p->Print_Statistics();

    // Print block info.
    while (true)
    {
        Block *b_p = grid_p->Get_Block(0);
        int lay = grid_p->Layer();

        cout << "------------------------------------------------------------------------"
             << "------------------------------------------" << endl;

        cout << "Ro :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].R;
        }
        cout << endl;

        cout << "Vx :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.X;
        }
        cout << endl;

        cout << "Vy :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.Y;
        }
        cout << endl;

        cout << "Vz :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].V.Z;
        }
        cout << endl;

        cout << "E  :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].E;
        }
        cout << endl;

        cout << "P  :";
        for (int i = 0; i < 10; i++)
        {
            cout << " " << setw(10) << b_p->Cells[i].U[lay].P;
        }
        cout << endl;

        cout << "------------------------------------------------------------------------"
             << "------------------------------------------" << endl;

        //char c = getc(stdin);

        break;
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

