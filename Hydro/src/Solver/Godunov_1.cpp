/**
 * \file
 * \brief Godunov Method order of accuracy 1.
 *
 * \author Alexey Rybakov
 */

#include "Godunov_1.h"
#include "Riemann.h"

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
    Fluid_Dyn_Pars u;
    Cell *c1_p = NULL;
    Cell *c2_p = NULL;
    double vo = 0.0;
    double d = 0.0;
    double sd = 0.0;

    b_p->Copy_Cur_Layer_To_Nxt();
    b_p->Nxt_Normal_To_Expand();

    for (int i = 0; i < i_size; i++)
    {
        for (int j = 0; j < j_size; j++)
        {
            for (int k = 0; k < k_size; k++)
            {
                c1_p = b_p->Get_Cell(i, j, k);
                vo = c1_p->Vo;
                d = dt / vo;

                // I0 direction (x-).
                if (i == 0)
                {
                    // Hard border, no flow.
                }

                // I1 direction (x+).
                if (i == i_size - 1)
                {
                    // Hard border, no flow.
                }
                else
                {
                    c2_p = b_p->Get_Cell(i + 1, j, k);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);
                    sd = c1_p->S[Direction::I1] * d;

                    double d_r = u.R * u.V.X * sd;
                    double d_rv = (u.R * u.V.X * u.V.X + u.P) * sd;
                    double d_e = u.V.X * (u.P + u.E) * sd;

                    c1_p->U[nxt].Flow_To_X(c2_p->U[nxt], d_r, d_rv, d_e);
                }

                // J0 direction (y-).
                if (j == 0)
                {
                    // Hard border, no flow.
                }

                // J1 direction (y+).
                if (j == j_size - 1)
                {
                    // Hard border, no flow.
                }
                else
                {
                    c2_p = b_p->Get_Cell(i, j + 1, k);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);
                    sd = c1_p->S[Direction::J1] * d;

                    double d_r = u.R * u.V.Y * sd;
                    double d_rv = (u.R * u.V.Y * u.V.Y + u.P) * sd;
                    double d_e = u.V.Y * (u.P + u.E) * sd;

                    c1_p->U[nxt].Flow_To_Y(c2_p->U[nxt], d_r, d_rv, d_e);
                }

                // K0 direction (z-).
                if (k == 0)
                {
                    // Hard border, no flow.
                }

                // K1 direction (z+).
                if (k == k_size - 1)
                {
                    // Hard border, no flow.
                }
                else
                {
                    c2_p = b_p->Get_Cell(i, j, k + 1);
                    Riemann::Avg(&c1_p->U[cur], &c2_p->U[cur], &u);
                    sd = c1_p->S[Direction::K1] * d;

                    double d_r = u.R * u.V.Z * sd;
                    double d_rv = (u.R * u.V.Z * u.V.Z + u.P) * sd;
                    double d_e = u.V.Z * (u.P + u.E) * sd;

                    c1_p->U[nxt].Flow_To_Y(c2_p->U[nxt], d_r, d_rv, d_e);
                }
            }
        }
    }

    // Restore real speed vector.
    b_p->Nxt_Expand_To_Normal();
}

} }

