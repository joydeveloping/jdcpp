/**
 * \file
 * \brief General Block description.
 *
 * \author Alexey Rybakov
 */

#include "Lib/IO/io.h"

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
    int Get_Id() const { return Id_; }
    int Get_I_Size() const { return I_Size_; }
    int Get_J_Size() const { return J_Size_; }
    int Get_K_Size() const { return K_Size_; }

private:

    // Identifier.
    int Id_;

    // I, J, K sizes.
    int I_Size_, J_Size_, K_Size_;

};

// Print information.
ostream &operator<<(ostream &os,
                    const Block *block_p);

} }

