/**
 * \file
 * \brief Facet in J direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_J.h"
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
void Facet_J::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int i = i_p->I0(); i < i_p->I1(); i++)
    {
        for (int k = i_p->K0(); k < i_p->K1(); k++)
        {
            Set_Border(i, k, i_p);
        }
    }
}

} }

