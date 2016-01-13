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
      NX_(NULL),
      NY_(NULL),
      NZ_(NULL),
      CX_(NULL),
      CY_(NULL),
      CZ_(NULL),
      VX_(NULL),
      VY_(NULL),
      VZ_(NULL),
      T_(NULL),
      Ro_(NULL),
      P_(NULL)
{
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

    return (3 * Nodes_Count() + 9 * Cells_Count()) * sizeof(float);
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

    NX_ = new float[nodes_count];
    NY_ = new float[nodes_count];
    NZ_ = new float[nodes_count];
    CX_ = new float[cells_count];
    CY_ = new float[cells_count];
    CZ_ = new float[cells_count];
    VX_ = new float[cells_count];
    VY_ = new float[cells_count];
    VZ_ = new float[cells_count];
    T_ = new float[cells_count];
    Ro_ = new float[cells_count];
    P_ = new float[cells_count];

    return (NX_ != NULL)
           && (NY_ != NULL)
           && (NZ_ != NULL)
           && (CX_ != NULL)
           && (CY_ != NULL)
           && (CZ_ != NULL)
           && (VX_ != NULL)
           && (VY_ != NULL)
           && (VZ_ != NULL)
           && (T_ != NULL)
           && (Ro_ != NULL)
           && (P_ != NULL);
}

/**
 * \brief Deallocate memory.
 */
void Block::Deallocate_Memory()
{
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

    if (CX_ != NULL)
    {
        delete CX_;
    }

    if (CY_ != NULL)
    {
        delete CY_;
    }

    if (CZ_ != NULL)
    {
        delete CZ_;
    }

    if (VX_ != NULL)
    {
        delete VX_;
    }

    if (VY_ != NULL)
    {
        delete VY_;
    }

    if (VZ_ != NULL)
    {
        delete VZ_;
    }

    if (T_ != NULL)
    {
        delete T_;
    }

    if (Ro_ != NULL)
    {
        delete Ro_;
    }

    if (P_ != NULL)
    {
        delete P_;
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

