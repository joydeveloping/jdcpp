/**
 * \file
 * \brief MPI tests.
 *
 * \author Alexey Rybakov
 */

#include "Lib/MPI/mpi.h"
#include "Lib/IO/io.h"
#include <cassert>

/*
 * Prototypes.
 */
void Test_N_To_N_Exchange();
void Test_N_To_0_To_N_Exchange();

/**
 * \brief Enter point.
 *
 * \param[in] argc - arguments count
 * \param[in] argv - arguments
 *
 * \return
 * Status.
 */
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // Analyze test.
    assert(argc == 2);
    string test(argv[1]);
    if (test == "n_to_n_exchange")
    {
        Test_N_To_N_Exchange();
    }
    else if (test == "n_to_0_to_n_exchange")
    {
        Test_N_To_0_To_N_Exchange();
    }

    MPI_Finalize();

    return 0;
}

/**
 * \brief Test exchange data N to N.
 *
 * We have N processes.
 * Each process has array of data (N * size * int).
 * Each process initializes its part of data;
 * then it send data to other processes.
 *
 * p0 : [0..0|*..*| ... |*..*]         [p0 -> __ p1 ... pn]     p0 : [0..0|1..1| ... |n..n]
 * p1 : [*..*|1..1| ... |*..*] --> MPI [p1 -> p0 __ ... pn] --> p1 : [0..0|1..1| ... |n..n]
 * ...                                 ...                      ...
 * pn : [*..*|*..*| ... |n..n]         [pn -> p0 p1 ... __]     pn : [0..0|1..1| ... |n..n]
 */
void Test_N_To_N_Exchange()
{
    const int size = 10000;
    const int rank = Lib::MPI::Rank();
    const int ranks_count = Lib::MPI::Ranks_Count();
    const int full_size = ranks_count * size;
    const int iters = 1000;
    int *data = new int[full_size];
    Lib::MPI::Timer *timer = new Lib::MPI::Timer();
    MPI_Request *reqs = new MPI_Request[2 * ranks_count];
    MPI_Status *stats = new MPI_Status[2 * ranks_count];
    int reqs_count = 0;

    // Iterations.
    for (int iter = 0; iter < iters; iter++)
    {
        // Init.
        for (int i = 0; i < full_size; i++)
        {
            data[i] = -1;
        }
        for (int i = 0; i < size; i++)
        {
            data[rank * size + i] = rank;
        }

        // Exchange.
        timer->Start();
        reqs_count = 0;
        for (int i = 0; i < ranks_count; i++)
        {
            if (i == rank)
            {
                continue;
            }

            // MPI requests.
            MPI_Irecv(static_cast<void *>(&data[i * size]), size, MPI_INT,
                      i, i, MPI_COMM_WORLD, &reqs[reqs_count++]);
            MPI_Isend(static_cast<void *>(&data[rank * size]), size, MPI_INT,
                      i, rank, MPI_COMM_WORLD, &reqs[reqs_count++]);
        }
        MPI_Waitall(reqs_count, reqs, stats);
        timer->Stop();

        // Check.
        for (int r = 0; r < ranks_count; r++)
        {
            for (int i = 0; i < size; i++)
            {
                assert(data[r * size + i] == r);
            }
        }
    }

    // Print.
    cout << "Time : " << timer->Time() << endl;

    delete stats;
    delete reqs;
    delete timer;
    delete data;
}

/**
 * \brief Test exchange data N to N through 0 process.
 *
 * We have N processes.
 * Each process has array of data (N * size * int).
 * Each process initializes its part of data;
 * then it send data to 0-processes.
 * After this 0-process sends data to all processes.
 *
 * p0 : [0..0|*..*| ... |*..*]                              p0 : [0..0|1..1| ... |n..n]
 * p1 : [*..*|1..1| ... |*..*] --> MPI [p1 -> p0] --------> p1 : [*..*|1..1| ... |*..*]
 * ...                                 ...                  ...
 * pn : [*..*|*..*| ... |n..n]         [pn -> p0]           pn : [*..*|*..*| ... |*..*]
 *
 *                                                          p0 : [0..0|1..1| ... |n..n]
 *                             --> MPI [p0 -> p1 .. pn] --> p1 : [0..0|1..1| ... |n..n]
 *                                                          ...
 *                                                          pn : [0..0|1..1| ... |n..n]
 */
void Test_N_To_0_To_N_Exchange()
{
    const int size = 10000;
    const int rank = Lib::MPI::Rank();
    const int ranks_count = Lib::MPI::Ranks_Count();
    const int full_size = ranks_count * size;
    const int iters = 1000;
    int *data = new int[full_size];
    Lib::MPI::Timer *timer = new Lib::MPI::Timer();
    MPI_Request *reqs = new MPI_Request[ranks_count];
    MPI_Status *stats = new MPI_Status[ranks_count];
    int reqs_count = 0;

    // Iterations.
    for (int iter = 0; iter < iters; iter++)
    {
        // Init.
        for (int i = 0; i < full_size; i++)
        {
            data[i] = -1;
        }
        for (int i = 0; i < size; i++)
        {
            data[rank * size + i] = rank;
        }

        /*
         * Exchange.
         */

        timer->Start();

        // First phase.
        reqs_count = 0;
        if (rank == 0)
        {
            for (int i = 1; i < ranks_count; i++)
            {
                MPI_Irecv(static_cast<void *>(&data[i * size]), size, MPI_INT,
                                              i, i, MPI_COMM_WORLD, &reqs[reqs_count++]);
            }
        }
        else
        {
            MPI_Isend(static_cast<void *>(&data[rank * size]), size, MPI_INT,
                                          0, rank, MPI_COMM_WORLD, &reqs[reqs_count++]);
        }
        MPI_Waitall(reqs_count, reqs, stats);

        // Second phase.
        reqs_count = 0;
        if (rank == 0)
        {
            for (int i = 1; i < ranks_count; i++)
            {
                MPI_Isend(static_cast<void *>(&data[0]), ranks_count * size, MPI_INT,
                          i, 0, MPI_COMM_WORLD, &reqs[reqs_count++]);
            }
        }
        else
        {
            MPI_Irecv(static_cast<void *>(&data[0]), ranks_count * size, MPI_INT,
                      0, 0, MPI_COMM_WORLD, &reqs[reqs_count++]);
        }
        MPI_Waitall(reqs_count, reqs, stats);

        timer->Stop();

        // Check.
        for (int r = 0; r < ranks_count; r++)
        {
            for (int i = 0; i < size; i++)
            {
                assert(data[r * size + i] == r);
            }
        }
    }

    // Print.
    cout << "Time : " << timer->Time() << endl;

    delete stats;
    delete reqs;
    delete timer;
    delete data;
}

