/**
 * \file
 * \brief General Grid realization.
 *
 * \author Alexey Rybakov
 */

#include "Grid.h"
#include <fstream>

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Grid::Grid()
    : Blocks_Count_(0)
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

        if (p)
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
    if (Blocks_p_)
    {
        delete Blocks_p_;
        Blocks_p_ = NULL;
        Blocks_Count_ = 0;
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

    // Close files.
    file_pfg.close();
    file_ibc.close();

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
    int count = grid_p->Blocks_Count();

    os << "Grid(" << count << "):" << endl;

    for (int i = 0; i < count; i++)
    {
        Block *p = grid_p->Get_Block(i);

        if (p)
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

} }

