/**
 * \file
 * \brief General Grid description.
 *
 * \author Alexey Rybakov
 */

#include "Block.h"

namespace Hydro { namespace Grid {

/**
 * \brief Grid class.
 */
class Grid
{

public:

    /*
     * Grid interface.
     */

    // Constructors/destructors.
    Grid(int blocks_count);
    ~Grid();

    // Simple data.
    int Get_Blocks_Count() const { return Blocks_Count_; }
    Block *Get_Block(int n) const { return Blocks_p_[n]; }

private:

    // Count of blocks.
    int Blocks_Count_;

    // Blocks.
    Block **Blocks_p_;
};

// Print information.
ostream &operator<<(ostream &os,
                    const Grid *grid_p);

} }

