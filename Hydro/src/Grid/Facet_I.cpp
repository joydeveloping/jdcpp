/**
 * \file
 * \brief Facet in I direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_I.h"
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
void Facet_I::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int j = i_p->J0(); j < i_p->J1(); j++)
    {
        for (int k = i_p->K0(); k < i_p->K1(); k++)
        {
            Set_Border(j, k, i_p);
        }
    }
}

} }

