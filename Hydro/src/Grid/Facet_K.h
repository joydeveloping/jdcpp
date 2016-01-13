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
    Facet_K(int i_size,
            int j_size);
    ~Facet_K();

    // Simple characteristics.
    int I_Size() const { return I_Size_; }
    int J_Size() const { return J_Size_; }
    bool Is_Direction_I() const { return false; }
    bool Is_Direction_J() const { return false; }
    bool Is_Direction_K() const { return true; }
    int Size() const { return I_Size() * J_Size(); }

    // Information.
    void Print(ostream &os) const;

private:

    // Size.
    int I_Size_, J_Size_;

    // Linear index of border.
    int L(int i, int j) const { return i * J_Size() + j; }
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet_K *p);

} }

#endif

