/**
 * \file
 * \brief Facet in direction K description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FACET_K_H
#define HYDRO_GRID_FACET_K_H

#include "Facet.h"

namespace Hydro { namespace Grid {

/**
 * \brief Facet in K direction.
 */
class Facet_K : public Facet
{

public:

    // Constructors/descturcors.
    Facet_K(int i_size, int j_size) : Facet(i_size, j_size) {}
    ~Facet_K() {}

    // Simple characteristics.
    int I_Size() const { return Height(); }
    int J_Size() const { return Width(); }
    bool Is_Direction_I() const { return false; }
    bool Is_Direction_J() const { return false; }
    bool Is_Direction_K() const { return true; }

    // Set interface.
    void Set_Iface(Iface *i_p);
};

} }

#endif

