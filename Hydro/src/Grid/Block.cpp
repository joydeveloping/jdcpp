/**
 * \file
 * \brief General Block realization.
 *
 * \author Alexey Rybakov
 */

#include "mpi.h"
#include "Block.h"
#include "configure.h"

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
      Rank_(0),
      Cells(NULL),
      NX_(NULL),
      NY_(NULL),
      NZ_(NULL)
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
    int cells_count = Cells_Count();
    int nodes_count = Nodes_Count();

    Deallocate_Memory();

    Cells = new Cell[cells_count];
    NX_ = new double[nodes_count];
    NY_ = new double[nodes_count];
    NZ_ = new double[nodes_count];

    return (NX_ != NULL)
           && (NY_ != NULL)
           && (NZ_ != NULL);
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

    if (NX_ != NULL)
    {
        delete NX_;
    }

    if (NY_ != NULL)
    {
        delete NY_;
    }

    if (NZ_ != NULL)
    {
        delete NZ_;
    }
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
    int ind = 0;

    // Nodes coordinates.
    for (int k = 0; k <= k_size; k++)
    {
        for (int j = 0; j <= j_size; j++)
        {
            for (int i = 0; i <= i_size; i++)
            {
                ind = k * ((i_size + 1) * (j_size + 1)) + j * (i_size + 1) + i;

                NX_[ind] = di * i;
                NY_[ind] = dj * j;
                NZ_[ind] = dk * k;
            }
        }
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

