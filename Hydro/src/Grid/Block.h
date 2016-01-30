/**
 * \file
 * \brief General Block description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_BLOCK_H
#define HYDRO_GRID_BLOCK_H

#include "Lib/MPI/mpi.h"
#include "Lib/IO/io.h"
#include "Direction.h"
#include "Facet.h"
#include "Facet_I.h"
#include "Facet_J.h"
#include "Facet_K.h"
#include "Cell.h"

using namespace std;

namespace Hydro { namespace Grid {

class Grid;

/**
 * \brief Block class.
 */
class Block
{

public:

    // Points.
    Point_3D *Nodes;

    // Cells.
    Cell *Cells;

    /*
     * Block interface.
     */

    // Constructors/destructors.
    Block(Grid *grid_p,
          int id,
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
    int Facet_Size(int direction) const { return Facets_p_[direction]->Size(); }
    int Surface_Area() const;
    int Iface_Cells_Count() const;
    int Shadow_Cells_Count() const;
    int Inner_Cells_Count() const;
    int Border_Cells_Count() const { Cells_Count() - Inner_Cells_Count(); };
    int Rank() const { return Rank_; }
    bool Is_Active() const { return Rank() == Lib::MPI::Rank(); }
    void Set_Rank(int rank) { Rank_ = rank; }
    Facet *Get_Facet(int i) const { return Facets_p_[i]; }
    Grid *Get_Grid() const { return Grid_p_; }

    // Allocate/deallocate memory.
    bool Allocate_Memory();
    void Deallocate_Memory();

    // Construct block.
    void Create_Solid_Descartes(double i_real_size,
                                double j_real_size,
                                double k_real_size);

    // Get node and cell pointers.
    Point_3D *Get_Node(int i, int j, int k);
    Cell *Get_Cell(int i, int j, int k);

    // Other.
    void Copy_Cur_Layer_To_Nxt();
    void Nxt_Normal_To_Expand();
    void Nxt_Expand_To_Normal();

private:

    // Grid.
    Grid *Grid_p_;

    // Identifier.
    int Id_;

    // I, J, K sizes.
    // Size is count of cells in given direction.
    int I_Size_, J_Size_, K_Size_;

    // Process rank.
    int Rank_;

    // Facets.
    Facet *Facets_p_[Direction::Count];

    // Init.
    void Create_Facets();
    void Destroy_Facets();

};

// Print information.
ostream &operator<<(ostream &os,
                    const Block *p);

} }

#endif

