/**
 * \file
 * \brief Facet in K direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_K.h"
#include "Iface.h"

namespace Hydro { namespace Grid {

/*
 * Set interface.
 */

/**
 * \brief Set interface.
 *
 * \param[in] i_p - interface pointer
 */
void Facet_K::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int i = i_p->I0(); i < i_p->I1(); i++)
    {
        for (int j = i_p->J0(); j < i_p->J1(); j++)
        {
            Set_Border(i, j, i_p);
        }
    }
}

} }

