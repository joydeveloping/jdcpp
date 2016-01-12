/**
 * \file
 * \brief Iface realization.
 *
 * \author Alexey Rybakov.
 */

#include <cassert>
#include <cmath>
#include "mpi.h"
#include "Iface.h"
#include "configure.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] id - identifier
 * \param[in] b_p - block pointer
 * \param[in] i0 - first node in i direction
 * \param[in] i1 - last node in i direction
 * \param[in] j0 - first node in j direction
 * \param[in] j1 - last node in j direction
 * \param[in] k0 - first node in k direction
 * \param[in] k1 - last node in k direction
 * \param[in] nb_p - neighbour block pointer
 */
Iface::Iface(int id,
             Block *b_p,
             int i0,
             int i1,
             int j0,
             int j1,
             int k0,
             int k1,
             Block *nb_p)
    : Id_(id),
      B_p_(b_p),
      I0_(i0),
      I1_(i1),
      J0_(j0),
      J1_(j1),
      K0_(k0),
      K1_(k1),
      NB_p_(nb_p),
      Buffer_p_(NULL)
{
    if (Is_Active())
    {
        Allocate_Buffer();
    }
}

/**
 * \brief Default destructor.
 */
Iface::~Iface()
{
    Deallocate_Buffer();
}

/*
 * Alocate/deallocate memory.
 */

/**
 * \brief Allocate memory for buffer.
 *
 * \return
 * true - if memory is allocated,
 * false - if memory is not allocated.
 */
bool Iface::Allocate_Buffer()
{
    Deallocate_Buffer();
    Buffer_p_ = new float[Buffer_Floats_Count()];

    return Buffer_p_ != NULL;
}

/**
 * \brief Deallocate memory.
 */
void Iface::Deallocate_Buffer()
{
    if (Buffer_p_ != NULL)
    {
        delete Buffer_p_;
        Buffer_p_ = NULL;
    }
}

/*
 * Simple data and characteristics.
 */

/**
 * \brief Check is interfaces used for MPI.
 *
 * \return
 * true - if interface is used for MPI,
 * fasle - if interface is not used for MPI.
 */
bool Iface::Is_MPI() const
{
    bool is_b = Is_BActive();
    bool is_n = Is_NActive();

    return (is_b && !is_n) || (!is_b && is_n);
}

/**
 * \brief Take count of cells in interface.
 *
 * \return
 * Count of cells in interface.
 */
int Iface::Cells_Count() const
{
    int di = I1_ - I0_;
    int dj = J1_ - J0_;
    int dk = K1_ - K0_;

    if (di == 0)
    {
        return dj * dk;
    }
    else if (dj == 0)
    {
        return di * dk;
    }
    else
    {
        assert(dk == 0);

        return di * dj;
    }
}

/**
 * \brief Take buffer cells count.
 *
 * \return Buffer cells count.
 */
int Iface::Buffer_Cells_Count() const
{
    if (Is_Active())
    {
        return Cells_Count() * HYDRO_GRID_SHADOW_DEPTH;
    }
    else
    {
        return 0;
    }
}

/*
 * Information.
 */

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] p - interface pointer
 */
ostream &operator<<(ostream &os,
                    const Iface *p)
{
    if (p->Is_Active())
    {
        cout << "  Iface("
             << setw(3) << p->Id() << "): "
             << setw(3) << p->B()->Id() << "["
             << setw(5) << p->I0() << ","
             << setw(5) << p->I1() << ","
             << setw(5) << p->J0() << ","
             << setw(5) << p->J1() << ","
             << setw(5) << p->K0() << ","
             << setw(5) << p->K1() << "] -> "
             << setw(3) << p->NB()->Id() << ", m["
             << setw(8) << p->Buffer_Bytes_Count() << "]" << endl;
    }
}

/*
 * Additional functions.
 */

/**
 * \brief Set buffer with single value v.
 *
 * \param[in] v - value
 */
void Iface::Set_Buffer_Value(float v)
{
    int n = Buffer_Floats_Count();

    for (int i = 0; i < n; i++)
    {
        Buffer_p_[i] = v;
    }
}

/**
 * \brief Check if all values in buffer are equal to given value.
 *
 * \return
 * true - if all values in buffer are equal to given value,
 * false - if not all values in buffer are equal to given value.
 */
bool Iface::Check_Buffer_Value(float v, float eps)
{
    int n = Buffer_Floats_Count();

    for (int i = 0; i < n; i++)
    {
        float b = Buffer_p_[i];

        if (fabs(b - v) > eps)
        {
            return false;
        }
    }

    return true;
}

} }

