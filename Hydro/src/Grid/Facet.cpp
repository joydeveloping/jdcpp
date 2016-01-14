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
 * \param[in] height - height
 * \param[in] width - width
 */
Facet::Facet(int height,
             int width)
    : Height_(height),
      Width_(width),
      Borders_p_(NULL)
{
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
 * Information.
 */

/**
 * \brief Check if interface in facet.
 *
 * \param[in] i - i coordinate
 * \param[in] j - j coordinate
 *
 * \return
 * true - if it is interface,
 * false - if it is not interface.
 */
bool Facet::Is_Iface(int i,
                     int j) const
{
    Border *p = Borders_p_[L(i, j)];

    if (p == NULL)
    {
        return false;
    }

    return p->Is_Iface();
}

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
 * \brief Index linearization.
 *
 * \param[in] i - i index
 * \param[in] j - j index
 *
 * \return
 * Linear index.
 */
int Facet::L(int i, int j) const
{
    int lin = i * Width() + j;

    assert(lin < Size());

    return lin;
}

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 */
void Facet::Print(ostream &os) const
{
    os << "    Facet:" << endl;

    for (int i = 0; i < Height(); i++)
    {
        os << "      ";
        for (int j = 0; j < Width(); j++)
        {
            os << Symbol(L(i, j));
        }
        os << endl;
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
    p->Print(os);
}

} }

