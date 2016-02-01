/**
 * \file
 * \brief Godunov Method order of accuracy 1.
 *
 * \author Alexey Rybakov
 */

#include "Godunov_1.h"
#include "Riemann.h"
#include "Lib/OMP/omp.h"

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
 * \brief Iterations calculation.
 *
 * \param[in] count - iterations count
 * \param[in] dt - time step
 */
void Godunov_1::Calc_Iters(int count,
                           double dt)
{
    for (int i = 0; i < count; i++)
    {
        Calc_Iter(dt);
    }
}

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

    G_p_->Swap_Layers();
}

/**
 * \brief Iteration calculation for single block.
 *
 * \param[in,out] b_p - block pointer
 * \param[in] dt - time step
 *
 * \TODO:
 * We suggest i is x Descartes coordinate,
 *            j is y Descartes coordinate,
 *            k is Z Descartes coordinate.
 */
void Godunov_1::Calc_Iter(Block *b_p,
                          double dt)
{
    int i_size = b_p->I_Size();
    int j_size = b_p->J_Size();
    int k_size = b_p->K_Size();
    int cur = b_p->Get_Grid()->Layer();
    int nxt = cur ^ 1;

    b_p->Copy_Cur_Layer_To_Nxt();
    b_p->Nxt_Normal_To_Expand();

    #pragma omp parallel for
    for (int i = 0; i < i_size; i++)
    {
        for (int j = 0; j < j_size; j++)
        {
            for (int k = 0; k < k_size; k++)
            {
                Cell *c1_p = b_p->Get_Cell(i, j, k);
                Cell *c2_p = NULL;
                Fluid_Dyn_Pars u;
                double sd = 0.0;
                double d = dt / c1_p->Vo;

                // I.

                // I0 direction (x-).
                sd = c1_p->S[Direction::I0] * d;
                if (i == 0)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.X *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_X(-u.DR_X() * sd, -u.DV_X() * sd, -u.DE_X() * sd);
                }

                // I1 direction (x+).
                sd = c1_p->S[Direction::I1] * d;
                if (i == i_size - 1)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.X *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_X(u.DR_X() * sd, u.DV_X() * sd, u.DE_X() * sd);
                }
                else
                {
                    c2_p = b_p->Get_Cell(i + 1, j, k);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);

                    c1_p->U[nxt].Flow_X(c2_p->U[nxt], u.DR_X() * sd, u.DV_X() * sd, u.DE_X() * sd);
                }

                // J.

                // J0 direction (y-).
                sd = c1_p->S[Direction::J0] * d;
                if (j == 0)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.Y *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_Y(-u.DR_Y() * sd, -u.DV_Y() * sd, -u.DE_Y() * sd);
                }

                // J1 direction (y+).
                sd = c1_p->S[Direction::J1] * d;
                if (j == j_size - 1)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.Y *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_Y(u.DR_Y() * sd, u.DV_Y() * sd, u.DE_Y() * sd);
                }
                else
                {
                    c2_p = b_p->Get_Cell(i, j + 1, k);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);

                    c1_p->U[nxt].Flow_Y(c2_p->U[nxt], u.DR_Y() * sd, u.DV_Y() * sd, u.DE_Y() * sd);
                }

                // K.

                // K0 direction (z-).
                sd = c1_p->S[Direction::K0] * d;
                if (k == 0)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.Z *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_Z(-u.DR_Z() * sd, -u.DV_Z() * sd, -u.DE_Z() * sd);
                }

                // K1 direction (z+).
                sd = c1_p->S[Direction::K1] * d;
                if (k == k_size - 1)
                {
                    // Hard border.

                    Fluid_Dyn_Pars u2 = c1_p->U[cur];

                    u2.V.Z *= -1.0;
                    Riemann::Avg(&c1_p->U[cur], &u2, &u);

                    c1_p->U[nxt].Flow_Z(u.DR_Z() * sd, u.DV_Z() * sd, u.DE_Z() * sd);
                }
                else
                {
                    c2_p = b_p->Get_Cell(i, j, k + 1);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);

                    c1_p->U[nxt].Flow_Z(c2_p->U[nxt], u.DR_Z() * sd, u.DV_Z() * sd, u.DE_Z() * sd);
                }
            }
        }
    }

    // Restore real speed vector.
    b_p->Nxt_Expand_To_Normal();
}

} }

