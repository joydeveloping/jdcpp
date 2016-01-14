/**
 * \file
 * \brief Facet in direction I description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FACET_I_H
#define HYDRO_GRID_FACET_I_H

#include "Facet.h"

namespace Hydro { namespace Grid {

/**
 * \brief Facet in I direction.
 */
class Facet_I : public Facet
{

public:

    // Constructors/descturcors.
    Facet_I(int j_size, int k_size) : Facet(j_size, k_size) {}

    // Simple characteristics.
    int J_Size() const { return Height(); }
    int K_Size() const { return Width(); }
    bool Is_Direction_I() const { return true; }
    bool Is_Direction_J() const { return false; }
    bool Is_Direction_K() const { return false; }

    // Set interface.
    void Set_Iface(Iface *i_p);
};

} }

#endif

