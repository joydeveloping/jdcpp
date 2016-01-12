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
    Grid();
    ~Grid();

    // Simple data.
    int Blocks_Count() const { return Blocks_Count_; }
    bool Is_Empty() const { return Blocks_Count() == 0; }
    Block *Get_Block(int n) const { return Blocks_p_[n]; }

    // Load and create Grid.
    bool Load_GEOM(const string name, int ranks_count);

    // Blocks ranks balancing.
    void Set_Blocks_Ranks_Circular_Distribution(int ranks_count);
    void Set_Blocks_Ranks_Cells_Balancing(int ranks_count);

private:

    // Count of blocks.
    int Blocks_Count_;

    // Blocks.
    Block **Blocks_p_;

    // Functions for Grid creation.
    bool Allocate_Blocks_Pointers(int count);
    void Deallocate_Blocks();
    void Deallocate_Blocks_Pointers();
};

// Print information.
ostream &operator<<(ostream &os,
                    const Grid *grid_p);

} }
