/**
 * \file
 * \brief General Block realization.
 *
 * \author Alexey Rybakov
 */

#include "Block.h"

namespace Hydro { namespace Grid {

/*
 * Block interface.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] id - identifier
 * \param[in] i_size - I size
 * \param[in] j_size - J size
 * \param[in] k_size - K size
 */
Block::Block(int id,
             int i_size,
             int j_size,
             int k_size)
    : Id_(id),
      I_Size_(i_size),
      J_Size_(j_size),
      K_Size_(k_size)
{
}

/**
 * \brief Default destructor.
 */
Block::~Block()
{
}

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] block_p - Block pointer
 */
ostream &operator<<(ostream &os,
                    const Block *block_p)
{
    cout << "  Block(" << block_p->Get_Id() << "): "
         << block_p->Get_I_Size() << ", "
         << block_p->Get_J_Size() << ", "
         << block_p->Get_K_Size() << endl;
}

} }

