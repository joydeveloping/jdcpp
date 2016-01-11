/**
 * \file
 * \brief General Grid realization.
 *
 * \author Alexey Rybakov
 */

#include "Grid.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] blocks_count - count of blocks
 */
Grid::Grid(int blocks_count)
    : Blocks_Count_(blocks_count)
{
    Blocks_p_ = new Block *[blocks_count];

    // TODO:
    // Just create blocks.
    // We have to load the grid from the file.
    for (int i = 0; i < blocks_count; i++)
    {
        Blocks_p_[i] = new Block(i, 5, 5, 5);
    }
}


/**
 * \brief Default destructor.
 */
Grid::~Grid()
{
    // Delete blocks.
    for (int i = 0; i < Get_Blocks_Count(); i++)
    {
        delete Blocks_p_[i];
    }

    delete Blocks_p_;
    Blocks_p_ = NULL;
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
    os << "Grid:" << endl;

    for (int i = 0; i < grid_p->Get_Blocks_Count(); i++)
    {
        os << grid_p->Get_Block(i);
    }
}

} }

