/**
 * \file
 * \brief General Block realization.
 *
 * \author Alexey Rybakov
 */

#include "mpi.h"
#include "Block.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
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
      K_Size_(k_size),
      Rank_(0)
{
}

/**
 * \brief Default destructor.
 */
Block::~Block()
{
}

/*
 * Simple data and characteristics.
 */

/**
 * \brief Check if active.
 *
 * \return
 * true - if block is active,
 * false - if block is not active.
 */
bool Block::Is_Active() const
{
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return Rank() == rank;
}

/*
 * Information.
 */

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] block_p - Block pointer
 */
ostream &operator<<(ostream &os,
                    const Block *block_p)
{
    if (block_p->Is_Active())
    {
        cout << "  Block("
             << setw(3) << block_p->Id() << ","
            << setw(3) << block_p->Rank() << "): ["
            << setw(5) << block_p->I_Size() << ", "
            << setw(5) << block_p->J_Size() << ", "
            << setw(5) << block_p->K_Size() << "]" << endl;
    }
}

} }

