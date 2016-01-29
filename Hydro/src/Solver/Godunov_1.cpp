/**
 * \file
 * \brief Godunov Method order of accuracy 1.
 *
 * \author Alexey Rybakov
 */

#include "Godunov_1.h"

namespace Hydro { namespace Solver {

/**
 * \brief Default constructor.
 *
 * \param[in] g_p - grid pointer
 */
Godunov_1::Godunov_1(Hydro::Grid::Grid *g_p)
    : G_p_(g_p)
{
}

/*
 * Calculations.
 */

/**
 * \brief Iteration calculation.
 *
 * \param[in] dt - time step
 */
void Godunov_1::Calc_Iter(double dt)
{
    for (int i = 0; i < G_p_->Blocks_Count(); i++)
    {
        Calc_Iter(G_p_->Get_Block(i), dt);
    }
}

/**
 * \brief Iteration calculation for single block.
 *
 * \param[in,out] b_p - block pointer
 * \param[in] dt - time step
 */
void Godunov_1::Calc_Iter(Block *b_p,
                          double dt)
{
    int i_size = b_p->I_Size();
    int j_size = b_p->J_Size();
    int k_size = b_p->K_Size();

    for (int i = 0; i < i_size; i++)
    {
        for (int j = 0; j < j_size; j++)
        {
            for (int k = 0; k < k_size; k++)
            {
                //
            }
        }
    }
}

} }
