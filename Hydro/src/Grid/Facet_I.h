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
    Facet_I(int j_size,
            int k_size);
    ~Facet_I();

    // Simple characteristics.
    int J_Size() const { return J_Size_; }
    int K_Size() const { return K_Size_; }
    bool Is_Direction_I() const { return true; }
    bool Is_Direction_J() const { return false; }
    bool Is_Direction_K() const { return false; }
    int Size() const { return J_Size() * K_Size(); }

    // Information.
    void Print(ostream &os) const;

private:

    // Size.
    int J_Size_, K_Size_;

    // Linear index of border.
    int L(int j, int k) const { return j * K_Size() + k; }
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet_I *p);

} }

#endif

