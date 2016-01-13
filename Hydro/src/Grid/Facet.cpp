/**
 * \file
 * \brief Facet realization.
 *
 * \author Alexey Rybakov
 */

#include <cassert>
#include "Lib/IO/io.h"
#include "Facet.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] i_size - I size
 * \param[in] j_size - J size
 * \param[in] k_size - K size
 */
Facet::Facet(int i_size,
             int j_size,
             int k_size)
    : I_Size_(i_size),
      J_Size_(j_size),
      K_Size_(k_size),
      Borders_p_(NULL)
{
    int zc = 0;

    if (i_size == 0)
    {
        zc++;
    }

    if (j_size == 0)
    {
        zc++;
    }

    if (k_size == 0)
    {
        zc++;
    }

    assert(zc == 1);

    Allocate_Memory();
}

/**
 * \brief Default destructor.
 */
Facet::~Facet()
{
    Deallocate_Memory();
}

/**
 * \brief Allocate memory.
 */
void Facet::Allocate_Memory()
{
    Deallocate_Memory();

    Borders_p_ = new Border *[Size()];

    for (int i = 0; i < Size(); i++)
    {
        Borders_p_[i] = NULL;
    }
}

/**
 * \brief Deallocate memory.
 */
void Facet::Deallocate_Memory()
{
    if (Borders_p_ != NULL)
    {
        delete Borders_p_;
    }
}

/*
 * Simple data and characteristics.
 */

/**
 * \brief Facet size in cells.
 *
 * \return
 * Count of cells.
 */
int Facet::Size() const
{
    if (Is_Direction_I())
    {
        return J_Size() * K_Size();
    }
    else if (Is_Direction_J())
    {
        return I_Size() * K_Size();
    }
    else
    {
        return I_Size() * J_Size();
    }
}

/*
 * Information.
 */

/**
 * \brief Get border symbol.
 *
 * \param[in] bi - border index
 *
 * \return
 * '0' - if no border,
 * 'I' - if border is interface
 * 'C' - if border is boundary condition
 */
char Facet::Symbol(int bi) const
{
    Border *p = Borders_p_[bi];

    if (p == NULL)
    {
        return '0';
    }
    else
    {
        if (p->Is_Iface())
        {
            return 'I';
        }
        else if (p->Is_BCond())
        {
            return 'C';
        }

        assert(false);
    }
}

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] p - facet pointer
 */
ostream &operator<<(ostream &os,
                    const Facet *p)
{
    int is = p->I_Size();
    int js = p->J_Size();
    int ks = p->K_Size();

    os << "    Facet:";

    if (p->Is_Direction_I())
    {
        os << "I:" << endl;

        for (int j = 0; j < js; j++)
        {
            os << "      ";
            for (int k = 0; k < ks; k++)
            {
                os << p->Symbol(j * ks + k);
            }
            os << endl;
        }
    }
    else if (p->Is_Direction_J())
    {
        os << "J:" << endl;

        for (int i = 0; i < is; i++)
        {
            os << "      ";
            for (int k = 0; k < ks; k++)
            {
                os << p->Symbol(i * ks + k);
            }
            os << endl;
        }
    }
    else if (p->Is_Direction_K())
    {
        os << "K:" << endl;

        for (int i = 0; i < is; i++)
        {
            os << "      ";
            for (int j = 0; j < js; j++)
            {
                os << p->Symbol(i * js + j);
            }
            os << endl;
        }
    }
}

} }

