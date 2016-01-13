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
 */
Facet::Facet()
    : Borders_p_(NULL)
{
}

/**
 * \brief Default destructor.
 */
Facet::~Facet()
{
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
    p->Print(os);
}

} }

