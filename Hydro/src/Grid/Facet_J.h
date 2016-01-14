/**
 * \file
 * \brief Facet in direction J description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FACET_J_H
#define HYDRO_GRID_FACET_J_H

#include "Facet.h"

namespace Hydro { namespace Grid {

/**
 * \brief Facet in J direction.
 */
class Facet_J : public Facet
{

public:

    // Constructors/descturcors.
    Facet_J(int i_size, int k_size) : Facet(i_size, k_size) {}
    ~Facet_J() {}

    // Simple characteristics.
    int I_Size() const { return Height(); }
    int K_Size() const { return Width(); }
    bool Is_Direction_I() const { return false; }
    bool Is_Direction_J() const { return true; }
    bool Is_Direction_K() const { return false; }

    // Set interface.
    void Set_Iface(Iface *i_p);
};

} }

#endif

