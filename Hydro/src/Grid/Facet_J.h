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
    Facet_J(int i_size,
            int k_size);
    ~Facet_J();

    // Simple characteristics.
    int I_Size() const { return I_Size_; }
    int K_Size() const { return K_Size_; }
    bool Is_Direction_I() const { return false; }
    bool Is_Direction_J() const { return true; }
    bool Is_Direction_K() const { return false; }
    int Size() const { return I_Size() * K_Size(); }

    // Information.
    void Print(ostream &os) const;

private:

    // Size.
    int I_Size_, K_Size_;

    // Linear index of border.
    int L(int i, int k) const { return i * K_Size() + k; }
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet_J *p);

} }

#endif

