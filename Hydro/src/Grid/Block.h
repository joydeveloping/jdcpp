/**
 * \file
 * \brief General Block description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_BLOCK_H
#define HYDRO_GRID_BLOCK_H

#include "Lib/IO/io.h"

using namespace std;

namespace Hydro { namespace Grid {

/**
 * \brief Block class.
 */
class Block
{

public:

    /*
     * Block interface.
     */

    // Constructors/destructors.
    Block(int id,
          int i_size,
          int j_size,
          int k_size);
    ~Block();

    // Simple characteristics.
    int Id() const { return Id_; }
    int I_Size() const { return I_Size_; }
    int J_Size() const { return J_Size_; }
    int K_Size() const { return K_Size_; }
    int Cells_Count() const { return I_Size() * J_Size() * K_Size(); }
    int I_Nodes() const { return I_Size() + 1; }
    int J_Nodes() const { return J_Size() + 1; }
    int K_Nodes() const { return K_Size() + 1; }
    int Nodes_Count() const { return I_Nodes() * J_Nodes() * K_Nodes(); }
    int Bytes_Count() const;
    int Rank() const { return Rank_; }
    bool Is_Active() const;
    void Set_Rank(int rank) { Rank_ = rank; }

    // Allocate/deallocate memory.
    bool Allocate_Memory();
    void Deallocate_Memory();

private:

    // Identifier.
    int Id_;

    // I, J, K sizes.
    // Size is count of cells in given direction.
    int I_Size_, J_Size_, K_Size_;

    // Process rank.
    int Rank_;

    /*
     * Area of static data.
     */

    // Nodes coordinates.
    float *NX_, *NY_, *NZ_;

    // Center coordinates.
    float *CX_, *CY_, *CZ_;

    /*
     * Area of dynamic data.
     */

    // Velocity vector.
    float *VX_, *VY_, *VZ_;

    // Hydrodynamic characteristics (temperature, density, pressure).
    float *T_, *Ro_, *P_;
};

// Print information.
ostream &operator<<(ostream &os,
                    const Block *p);

} }

#endif

