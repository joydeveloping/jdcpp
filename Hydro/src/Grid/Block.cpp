/**
 * \file
 * \brief General Block realization.
 *
 * \author Alexey Rybakov
 */

#include "mpi.h"
#include "Block.h"
#include "configure.h"
#include "Grid.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] grid_p - grid pointer
 * \param[in] id - identifier
 * \param[in] i_size - I size
 * \param[in] j_size - J size
 * \param[in] k_size - K size
 */
Block::Block(Grid *grid_p,
             int id,
             int i_size,
             int j_size,
             int k_size)
    : Grid_p_(grid_p),
      Id_(id),
      I_Size_(i_size),
      J_Size_(j_size),
      K_Size_(k_size),
      Rank_(0),
      Nodes(NULL),
      Cells(NULL)
{
    Allocate_Memory();

    for (int i = 0; i < Direction::Count; i++)
    {
        Facets_p_[i] = NULL;
    }

    Create_Facets();
}

/**
 * \brief Default destructor.
 */
Block::~Block()
{
    Destroy_Facets();
    Deallocate_Memory();
}

/**
 * \brief Create facets.
 */
void Block::Create_Facets()
{
    Destroy_Facets();

    Facets_p_[Direction::I0] = new Facet_I(J_Size(), K_Size());
    Facets_p_[Direction::I1] = new Facet_I(J_Size(), K_Size());
    Facets_p_[Direction::J0] = new Facet_J(I_Size(), K_Size());
    Facets_p_[Direction::J1] = new Facet_J(I_Size(), K_Size());
    Facets_p_[Direction::K0] = new Facet_K(I_Size(), J_Size());
    Facets_p_[Direction::K1] = new Facet_K(I_Size(), J_Size());
}

/**
 * \brief Destroy facets.
 */
void Block::Destroy_Facets()
{
    for (int i = 0; i < Direction::Count; i++)
    {
        if (Facets_p_[i] != NULL)
        {
            delete Facets_p_[i];
        }
    }
}

/*
 * Simple data and characteristics.
 */

/**
 * \brief Get bytes count used for block data.
 *
 * \return
 * Bytes count used for block data.
 */
int Block::Bytes_Count() const
{
    // NX_, NY_, NZ_ - nodes_count
    // CX_, CY_, CZ_, VX_, VY_, VZ_, T_, Ro_, P_ - cells_count

    return (3 * Nodes_Count() + 9 * Cells_Count()) * sizeof(double);
}

/**
 * \brief Surface area.
 *
 * \return
 * Surface area.
 */
int Block::Surface_Area() const
{
    int is = I_Size();
    int js = J_Size();
    int ks = K_Size();

    return 2 * (is * js + is * ks + js * ks);
}

/**
 * \brief Get interface cells count.
 *
 * \return
 * Count of interface cells.
 */
int Block::Iface_Cells_Count() const
{
    int is = I_Size();
    int js = J_Size();
    int ks = K_Size();
    int c = 0;
    int d = HYDRO_GRID_SHADOW_DEPTH;

    // We have to analyze all cells.
    for (int i = 0; i < is; i++)
    {
        for (int j = 0; j < js; j++)
        {
            for (int k = 0; k < ks; k++)
            {
                if (i < d)
                {
                    // Facet I0.
                    if (Get_Facet(Direction::I0)->Is_Iface(j, k))
                    {
                        c++;

                        continue;
                    }
                }

                if (i > is - 1 - d)
                {
                    // Facet I1.
                    if (Get_Facet(Direction::I1)->Is_Iface(j, k))
                    {
                        c++;

                        continue;
                    }
                }

                if (j < d)
                {
                    // Facet J0.

                    if (Get_Facet(Direction::J0)->Is_Iface(i, k))
                    {
                        c++;

                        continue;
                    }
                }

                if (j > js - 1 - d)
                {
                    if (Get_Facet(Direction::J1)->Is_Iface(i, k))
                    {
                        c++;

                        continue;
                    }
                }

                if (k < d)
                {
                    if (Get_Facet(Direction::K0)->Is_Iface(i, j))
                    {
                        c++;

                        continue;
                    }
                }

                if (k > ks - 1 - d)
                {
                    if (Get_Facet(Direction::K1)->Is_Iface(i, j))
                    {
                        c++;

                        continue;
                    }
                }
            }
        }
    }

    return c;
}

/**
 * \brief Get shadow cells count.
 *
 * \return
 * Count of shadow cells.
 */
int Block::Shadow_Cells_Count() const
{
    int is = I_Size();
    int js = J_Size();
    int ks = K_Size();
    int c = 0;
    int d = HYDRO_GRID_SHADOW_DEPTH;

    // We have to analyze all cells.
    for (int i = 0; i < is; i++)
    {
        for (int j = 0; j < js; j++)
        {
            for (int k = 0; k < ks; k++)
            {
                if (i < d)
                {
                    // Facet I0.
                    if (Get_Facet(Direction::I0)->Is_Iface(j, k))
                    {
                        c++;
                    }
                }

                if (i > is - 1 - d)
                {
                    // Facet I1.
                    if (Get_Facet(Direction::I1)->Is_Iface(j, k))
                    {
                        c++;
                    }
                }

                if (j < d)
                {
                    // Facet J0.

                    if (Get_Facet(Direction::J0)->Is_Iface(i, k))
                    {
                        c++;
                    }
                }

                if (j > js - 1 - d)
                {
                    if (Get_Facet(Direction::J1)->Is_Iface(i, k))
                    {
                        c++;
                    }
                }

                if (k < d)
                {
                    if (Get_Facet(Direction::K0)->Is_Iface(i, j))
                    {
                        c++;
                    }
                }

                if (k > ks - 1 - d)
                {
                    if (Get_Facet(Direction::K1)->Is_Iface(i, j))
                    {
                        c++;
                    }
                }
            }
        }
    }

    return c;
}

/**
 * \brief Get inner cells count.
 *
 * \return
 * Count of inner cells.
 */
int Block::Inner_Cells_Count() const
{
    int d2 = HYDRO_GRID_SHADOW_DEPTH * 2;
    int is = I_Size() - d2;
    int js = J_Size() - d2;
    int ks = K_Size() - d2;

    if ((is <= 0) || (js <= 0) || (ks <= 0))
    {
        return 0;
    }

    return is * js * ks;
}

/*
 * Allocate/deallocate memory.
 */

/**
 * \brief Allocate memory.
 *
 * \return
 * true - if memory is allocated,
 * false - if memory is not allocated.
 */
bool Block::Allocate_Memory()
{
    int nodes_count = Nodes_Count();
    int cells_count = Cells_Count();

    Deallocate_Memory();

    Nodes = new Point_3D[nodes_count];
    Cells = new Cell[cells_count];

    return (Nodes != NULL) && (Cells != NULL);
}

/**
 * \brief Deallocate memory.
 */
void Block::Deallocate_Memory()
{
    if (Cells != NULL)
    {
        delete Cells;
    }

    if (Nodes != NULL)
    {
        delete Nodes;
    }
}

/**
 * Get node and cell pointers.
 */

/**
 * \brief Get node pointer.
 *
 * \param[in] i - i coordinate
 * \param[in] j - j coordinate
 * \param[in] k - k coordinate
 *
 * \return
 * Node pointer.
 */
Point_3D *Block::Get_Node(int i,
                          int j,
                          int k)
{
    return &Nodes[(k * (J_Size() + 1) + j) * (I_Size() + 1) + i];
}

/**
 * \brief Get cell point.
 *
 * \param[in] i - i coordinate
 * \param[in] j - j coordinate
 * \param[in] k - k coordinate
 *
 * \return Cell pointer.
 */
Cell *Block::Get_Cell(int i,
                          int j,
                          int k)
{
    return &Cells[(k * J_Size() + j) * I_Size() + i];
}

/**
 * Construct block.
 */

/**
 * \brief Construct solid descartes block.
 *
 * \param[in] i_real_size - size in i direction
 * \param[in] j_real_size - size in j direction
 * \param[in] k_real_size - size in k direction
 */
void Block::Create_Solid_Descartes(double i_real_size,
                                   double j_real_size,
                                   double k_real_size)
{
    int i_size = I_Size();
    int j_size = J_Size();
    int k_size = K_Size();
    int cells_count = Cells_Count();
    int ij_size = i_size * j_size;
    double di = i_real_size / i_size;
    double dj = j_real_size / j_size;
    double dk = k_real_size / k_size;
    double vo = di * dj * dk;
    int cur = Get_Grid()->Layer();

    // Nodes coordinates.
    for (int i = 0; i <= i_size; i++)
    {
        for (int j = 0; j <= j_size; j++)
        {
            for (int k = 0; k <= k_size; k++)
            {
                Get_Node(i, j, k)->Set(di * i, dj * j, dk * k);
            }
        }
    }

    // Cells centers coordinates, volume and edges squares.
    for (int i = 0; i < i_size; i++)
    {
        for (int j = 0; j < j_size; j++)
        {
            for (int k = 0; k < k_size; k++)
            {
                Cell *c_p = Get_Cell(i, j, k);

                c_p->Set_Center(di * (i + 0.5), dj * (j + 0.5), dk * (k + 0.5));
                c_p->Vo = vo;
                c_p->Set_Descartes_Edges_Squares(dj * dk, di * dk, di * dj);
            }
        }
    }

    // Set U.
    for (int i = 0; i < i_size; i++)
    {
        for (int j = 0; j < j_size; j++)
        {
            for (int k = 0; k < k_size; k++)
            {
                Cell *c_p = Get_Cell(i, j, k);
                Fluid_Dyn_Pars *fdp_p = &c_p->U[cur];

                fdp_p->Ro = 1.225;
                fdp_p->V.X = 0.0;
                fdp_p->V.Y = 0.0;
                fdp_p->V.Z = 0.0;
                fdp_p->P = 101325.0;
                fdp_p->Eps = (1.0 / (1.4 - 1.0)) * fdp_p->P / fdp_p->Ro;
            }
        }
    }
}

/**
 * \brief Copy current layer to next.
 */
void Block::Copy_Cur_Layer_To_Nxt()
{
    int cur = Get_Grid()->Layer();
    int nxt = cur ^ 1;

    for (int i = 0; i < Cells_Count(); i++)
    {
        Cell *c_p = &Cells[i];

        c_p->U[nxt].Copy_From(&c_p->U[cur]);
    }
}

/**
 * \brief Move next layer to divergent form.
 */
void Block::Nxt_To_Divergent_Form()
{
    int cur = Get_Grid()->Layer();
    int nxt = cur ^ 1;

    for (int i = 0; i < Cells_Count(); i++)
    {
        Fluid_Dyn_Pars *p_p = &Cells[i].U[nxt];

        p_p->Eps = p_p->Ro * (0.5 * p_p->V.Mod_2() + p_p->Eps);
        p_p->V.X *= p_p->Ro;
        p_p->V.Y *= p_p->Ro;
        p_p->V.Z *= p_p->Ro;
    }
}


/**
 * \brief Extract next layer from divergent form.
 */
void Block::Nxt_From_Divergent_Form()
{
    int cur = Get_Grid()->Layer();
    int nxt = cur ^ 1;

    for (int i = 0; i < Cells_Count(); i++)
    {
        Fluid_Dyn_Pars *p_p = &Cells[i].U[nxt];

        p_p->V.X /= p_p->Ro;
        p_p->V.Y /= p_p->Ro;
        p_p->V.Z /= p_p->Ro;
        p_p->Eps = p_p->Eps / p_p->Ro - 0.5 * p_p->V.Mod_2();
        p_p->P = p_p->Eps * p_p->Ro * (1.4 - 1.0);
    }
}

/*
 * Information.
 */

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] p - Block pointer
 */
ostream &operator<<(ostream &os,
                    const Block *p)
{
    if (p->Is_Active())
    {
        os << "  Block("
           << setw(3) << p->Id() << ","
           << setw(3) << p->Rank() << "): s["
           << setw(5) << p->I_Size() << ", "
           << setw(5) << p->J_Size() << ", "
           << setw(5) << p->K_Size() << "], c["
           << setw(8) << p->Cells_Count() << "], f["
           << setw(8) << p->Surface_Area() << "], m["
           << setw(8) << p->Bytes_Count() << "]" << endl;

#if (HYDRO_GRID_IS_PRINT_FACETS)

        for (int i = 0; i < Direction::Count; i++)
        {
            os << p->Get_Facet(i);
        }

#endif

    }
}

} }

