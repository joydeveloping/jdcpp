/**
 * \file
 * \brief General Block description.
 *
 * \author Alexey Rybakov
 */

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
    int Rank() const { return Rank_; }
    bool Is_Active() const;
    void Set_Rank(int rank) { Rank_ = rank; }

private:

    // Identifier.
    int Id_;

    // I, J, K sizes.
    // Size is count of cells in given direction.
    int I_Size_, J_Size_, K_Size_;

    // Process rank.
    int Rank_;
};

// Print information.
ostream &operator<<(ostream &os,
                    const Block *block_p);

} }

