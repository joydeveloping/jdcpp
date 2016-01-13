/**
 * \file
 * \brief Facet description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FACET_H
#define HYDRO_GRID_FACET_H

#include "Lib/IO/io.h"
#include "Border.h"

namespace Hydro { namespace Grid {

/**
 * \brief Facet.
 */
class Facet
{

public:

    // Constructors/destructors.
    Facet(int i_size,
          int j_size,
          int k_size);
    ~Facet();

    // Simple data and characteristics.
    int I_Size() const { return I_Size_; }
    int J_Size() const { return J_Size_; }
    int K_Size() const { return K_Size_; }
    bool Is_Direction_I() const { return I_Size() == 0; }
    bool Is_Direction_J() const { return J_Size() == 0; }
    bool Is_Direction_K() const { return K_Size() == 0; }
    int Size() const;

    // Information.
    char Symbol(int bi) const;

private:

    // Sizes.
    int I_Size_, J_Size_, K_Size_;

    // Border pointers.
    Border **Borders_p_;

    // Memory managements.
    void Allocate_Memory();
    void Deallocate_Memory();
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet *p);

} }

#endif

