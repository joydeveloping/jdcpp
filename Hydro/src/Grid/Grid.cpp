/**
 * \file
 * \brief General Grid realization.
 *
 * \author Alexey Rybakov
 */

#include <fstream>
#include <cassert>
#include "mpi.h"
#include "Grid.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Grid::Grid()
    : Blocks_Count_(0),
      Ifaces_Count_(0)
{
    // Empty grid.
    Blocks_p_ = NULL;
}

/**
 * \brief Default destructor.
 */
Grid::~Grid()
{
    Deallocate_Blocks();
    Deallocate_Blocks_Pointers();
    Deallocate_Ifaces();
    Deallocate_Ifaces_Pointers();
}

/*
 * Allocate/deallocate blocks.
 */

/**
 * \brief Allocate blocks pointers.
 *
 * \param[in] count - blocks count
 *
 * \return
 * true - if blocks array is created (array of blocks pointers)<
 * false - if blocks array is not created.
 */
bool Grid::Allocate_Blocks_Pointers(int count)
{
    Deallocate_Blocks();
    Blocks_p_ = new Block *[count];
    Blocks_Count_ = count;

    return Blocks_p_ != NULL;
}

/**
 * \brief Deallocate blocks.
 */
void Grid::Deallocate_Blocks()
{
    for (int i = 0; i < Blocks_Count(); i++)
    {
        Block *p = Get_Block(i);

        if (p != NULL)
        {
            delete p;
        }
    }
}

/**
 * \brief Deallocate blocks pointers.
 */
void Grid::Deallocate_Blocks_Pointers()
{
    if (Blocks_p_ != NULL)
    {
        delete Blocks_p_;
        Blocks_p_ = NULL;
        Blocks_Count_ = 0;
    }
}

/**
 * \brief Allocate interfaces pointers.
 *
 * \param[in] count - count of interfaces
 *
 * \return
 * true - if interfaces pointers are allocated,
 * false - if interfaces pointers are not allocated.
 */
bool Grid::Allocate_Ifaces_Pointers(int count)
{
    Deallocate_Ifaces();
    Ifaces_p_ = new Iface *[count];
    Ifaces_Count_ = count;

    return Ifaces_p_ != NULL;
}

/**
 * \brief Deallocate interfaces.
 */
void Grid::Deallocate_Ifaces()
{
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p != NULL)
        {
            delete p;
        }
    }
}

/**
 * \brief Deallocate interfaces pointers.
 */
void Grid::Deallocate_Ifaces_Pointers()
{
    if (Ifaces_p_ != NULL)
    {
        delete Ifaces_p_;
        Ifaces_p_ = NULL;
        Ifaces_Count_ = 0;
    }
}

/*
 * Load/save Grid.
 */

/**
 * \brief Load data from GEOM format.
 *
 * \param[in] name - name of data
 * \param[in] ranks_count - count of ranks
 *
 * \return
 * true - if grid is loaded,
 * false - in other cases.
 */
bool Grid::Load_GEOM(const string name,
                     int ranks_count)
{
    string name_pfg = name + ".pfg";
    string name_ibc = name + ".ibc";
    ifstream file_pfg, file_ibc;

    file_pfg.open(name_pfg.c_str());
    if (!file_pfg.is_open())
    {
        cout << "Err: Cannot open file: " << name_pfg << endl;

        return false;
    }

    file_ibc.open(name_ibc.c_str());
    if (!file_ibc.is_open())
    {
        cout << "Err: Cannot open file: " << name_ibc << endl;

        return false;
    }

    int blocks_count;

    // Read blocks count and allocate memory for pointers.
    file_pfg >> blocks_count;
    Allocate_Blocks_Pointers(blocks_count);

    // Read all blocks.
    for (int i = 0; i < blocks_count; i++)
    {
        int i_size, j_size, k_size;

        // Read block sizes and create block.
        file_pfg >> i_size >> j_size >> k_size;
        Blocks_p_[i] = new Block(i, i_size - 1, j_size - 1, k_size - 1);
    }

    // Blocks balancing.
    Set_Blocks_Ranks_Cells_Balancing(ranks_count);

    // Allocate memory for blocks.
    for (int i = 0; i < blocks_count; i++)
    {
        Block *p = Get_Block(i);

        if (p->Is_Active())
        {
            p->Allocate_Memory();
        }
    }

    if (!Load_GEOM_Ifaces(file_ibc))
    {
        cout << "Err: Interfaces loading failed." << endl;
    }

    // Close files.
    file_pfg.close();
    file_ibc.close();

    return true;
}

/**
 * \brief Load Grid interfaces.
 *
 * \param[in] s - stream
 */
bool Grid::Load_GEOM_Ifaces(ifstream &s)
{
    string tmp;
    int ifaces_count, pos;

    // Pass two strings.
    getline(s, tmp);
    getline(s, tmp);

    // Read interfaces count and set up pointers.
    s >> ifaces_count;
    Allocate_Ifaces_Pointers(ifaces_count);

    // First set null pointers.
    for (int i = 0; i < ifaces_count; i++)
    {
        Ifaces_p_[i] = NULL;
    }

    // Read all interfaces.
    pos = 0;
    for (int iter = 0; iter < ifaces_count; iter++)
    {
        int id, bid, i0, i1, j0, j1, k0, k1, nid;

        // Read interface parameters and create interface.
        s >> id >> bid >> i0 >> i1 >> j0 >> j1 >> k0 >> k1 >> nid;

        // We are going to write interface to position "pos".
        // But if there is interface with the same id before,
        // then we have to write current interface after it.

        // Try to search i.
        int cur_pos = -1;
        for (int i = 0; i < ifaces_count; i++)
        {
            Iface *p = Get_Iface(i);

            if (p != NULL)
            {
                if (p->Id() == id)
                {
                    cur_pos = i;

                    break;
                }
            }
        }

        if (cur_pos != -1)
        {
            // We have this id before (and we have place for second interface with the same id).
            cur_pos++;
        }
        else
        {
            // Use position "pos".
            cur_pos = pos;
            pos += 2;
        }

        Ifaces_p_[cur_pos] = new Iface(id,
                                       Get_Block(bid - 1),
                                       i0, i1, j0, j1, k0, k1,
                                       Get_Block(nid - 1));
    }

    return true;
}

/*
 * Information.
 */

/**
 * \brief Print Grid information.
 *
 * \param[in] os - stream
 * \param[in] grid_ - Grid pointer
 */
ostream &operator<<(ostream &os,
                    const Grid *grid_p)
{
    int blocks_count = grid_p->Blocks_Count();
    int ifaces_count = grid_p->Ifaces_Count();

    os << "Grid(b" << blocks_count << ",i" << ifaces_count << "):" << endl;

    for (int i = 0; i < blocks_count; i++)
    {
        Block *p = grid_p->Get_Block(i);

        if (p != NULL)
        {
            os << p;
        }
    }

    for (int i = 0; i < ifaces_count; i++)
    {
        Iface *p = grid_p->Get_Iface(i);

        if (p != NULL)
        {
            os << p;
        }
    }
}

/*
 * Blocks ranks balancing.
 */

/**
 * \brief Circular distribution of blocks ranks.
 *
 * \param[in] ranks - ranks count
 */
void Grid::Set_Blocks_Ranks_Circular_Distribution(int ranks_count)
{
    for (int i = 0; i < Blocks_Count(); i++)
    {
        Get_Block(i)->Set_Rank(i % ranks_count);
    }
}

/**
 * \brief Balancing of blocks ranks.
 *
 * \param[in] ranks_count - count of ranks
 */
void Grid::Set_Blocks_Ranks_Cells_Balancing(int ranks_count)
{
    int blocks_count = Blocks_Count();

    if (Is_Empty())
    {
        return;
    }

    // Allocate counters of cells count for each rank.
    int *ranks_cells = new int[ranks_count];

    // Zero all counters.
    for (int i = 0; i < ranks_count; i++)
    {
        ranks_cells[i] = 0;
    }

    // Reset blocks ranks.
    for (int i = 0; i < blocks_count; i++)
    {
        Get_Block(i)->Set_Rank(-1);
    }

    // Now we put the biggest block without rank to the most empty rank.
    for (int iter = 0; iter < blocks_count; iter++)
    {
        int big_block_num = -1;
        int big_block_size = 0;

        for (int i = 0; i < blocks_count; i++)
        {
            Block *block_p = Get_Block(i);

            if (block_p->Rank() == -1)
            {
                int block_size = block_p->Cells_Count();

                if (block_size > big_block_size)
                {
                    big_block_num = i;
                    big_block_size = block_size;
                }
            }
        }

        int emp_rank_num = 0;

        for (int i = 1; i < ranks_count; i++)
        {
            if (ranks_cells[i] < ranks_cells[emp_rank_num])
            {
                emp_rank_num = i;
            }
        }

        // Put big_block_num block to emp_rank_num rank.
        Get_Block(big_block_num)->Set_Rank(emp_rank_num);
        ranks_cells[emp_rank_num] += big_block_size;
    }

    delete ranks_cells;
}

/*
 * Calculations.
 */

/**
 * \brief Calculate single iteration.
 */
void Grid::Calculate_Iteration()
{
    // Set predefined values to buffers.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p->Is_MPI())
        {
            if (p->Is_BActive())
            {
                p->Set_Buffer_Value(0.0);
            }
            else
            {
                p->Set_Buffer_Value(1.0);
            }
        }
    }

    // TODO:
    // Hydrodynamic calculations.

    // Check buffers.
    for (int i = 0; i < Ifaces_Count(); i++)
    {
        Iface *p = Get_Iface(i);

        if (p->Is_MPI())
        {
            if (p->Is_BActive())
            {
                assert(p->Check_Buffer_Value(1.0, 0.001));
            }
        }
    }
}

/**
 * \brief Calculate iterations.
 *
 * \param[in] n - iterations number
 */
void Grid::Calculate_Iterations(int n)
{
    for (int i = 0; i < n; i++)
    {
        Calculate_Iteration();
    }
}

/**
 * \brief MPI data exchange for interfaces.
 */
void Grid::Ifaces_MPI_Data_Exchange()
{
    MPI_Request reqs[100];
    MPI_Status stats[100];
    int reqs_count = 0;
    int i = 0;

    // Process all interfaces.
    while (i < Ifaces_Count())
    {
        Iface *p = Get_Iface(i);

        if (p->Is_BActive())
        {
            if (p->Is_NActive())
            {
                // Both blocks are active.
                // We can replace data without MPI.

                i += 2;
            }
            else
            {
                // Self block is active, neighbour is not.
                // We have to receive data from neighbour block process.

                MPI_Irecv(p->MPI_Buffer(), p->Buffer_Floats_Count(), MPI_FLOAT,
                          p->NB()->Rank(), p->Id(), MPI_COMM_WORLD, &reqs[reqs_count++]);
                i++;
            }
        }
        else
        {
            if (p->Is_NActive())
            {
                // Neighbour block is active, self is not.
                // We have to send data to self block process.
                MPI_Isend(p->MPI_Buffer(), p->Buffer_Floats_Count(), MPI_FLOAT,
                          p->B()->Rank(), p->Id(), MPI_COMM_WORLD, &reqs[reqs_count++]);
                i++;
            }
            else
            {
                // Self and neighbour blocks are not active.
                // Nothing to do.
                i += 2;
            }
        }

        // Wait all requests.
        MPI_Waitall(reqs_count, reqs, stats);
    }
}

} }

