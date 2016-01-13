/**
 * \file
 * \brief General Grid description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_GRID_H
#define HYDRO_GRID_GRID_H

#include "Lib/MPI/mpi.h"
#include "Iface.h"

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
    int Ifaces_Count() const { return Ifaces_Count_; }
    bool Is_Empty() const { return Blocks_Count() == 0; }
    Block *Get_Block(int n) const { return Blocks_p_[n]; }
    Iface *Get_Iface(int n) const { return Ifaces_p_[n]; }

    // Load and create Grid.
    bool Load_GEOM(const string name, int ranks_count);

    // Blocks ranks balancing.
    void Set_Blocks_Ranks_Circular_Distribution(int ranks_count);
    void Set_Blocks_Ranks_Cells_Balancing(int ranks_count);

    // Calculations.
    void Calculate_Iteration();
    void Calculate_Iterations(int n);

    // Timers.
    Lib::MPI::Timer *Timer_Shadow_Exchange() const { return Timer_Shadow_Exchange_p_; }

    // Information.
    void Print_Timers(ostream &os);
    void Print_Timers() { Print_Timers(cout); }

private:

    // Count of blocks.
    int Blocks_Count_;

    // Blocks.
    Block **Blocks_p_;

    // Count of interfaces.
    int Ifaces_Count_;

    // Interfaces.
    Iface **Ifaces_p_;

    // Timers.
    Lib::MPI::Timer *Timer_Shadow_Exchange_p_;

    // Init.
    void Init_Timers();

    // Functions for Grid creation.
    bool Allocate_Blocks_Pointers(int count);
    void Deallocate_Blocks();
    void Deallocate_Blocks_Pointers();
    bool Allocate_Ifaces_Pointers(int count);
    void Deallocate_Ifaces();
    void Deallocate_Ifaces_Pointers();

    // Load functions.
    bool Load_GEOM_Ifaces(ifstream &s);

    // Some help functions for iteration.
    void Ifaces_MPI_Data_Exchange();
};

// Print information.
ostream &operator<<(ostream &os,
                    const Grid *grid_p);

} }

#endif

