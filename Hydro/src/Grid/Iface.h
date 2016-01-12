/**
 * \file
 * \brief Interface (between blocks) description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_IFACE_H
#define HYDRO_GRID_IFACE_H

#include "Block.h"

namespace Hydro { namespace Grid {

/**
 * \brief Interface class.
 */
class Iface
{

public:

    // Constructors/destructors.
    Iface(int id,
          Block *b_p,
          int i0,
          int i1,
          int j0,
          int j1,
          int k0,
          int k1,
          Block *np_p);
    ~Iface();

    // Simple data and characteristics.
    int Id() const { return Id_; }
    Block *B() const { return B_p_; }
    int I0() const { return I0_; }
    int I1() const { return I1_; }
    int J0() const { return J0_; }
    int J1() const { return J1_; }
    int K0() const { return K0_; }
    int K1() const { return K1_; }
    bool Is_BActive() const { return B()->Is_Active(); }
    Block *NB() const { return NB_p_; }
    bool Is_NActive() const { return NB()->Is_Active(); }
    bool Is_Active() const { return Is_BActive() || Is_NActive(); }
    bool Is_MPI() const;
    int Cells_Count() const;
    int Buffer_Cells_Count() const;

private:

    // Identifier.
    int Id_;

    // Pointer to self block.
    Block *B_p_;

    // Coordinated of block interface area.
    int I0_, I1_, J0_, J1_, K0_, K1_;

    // Pointer to neighbour block.
    Block *NB_p_;

    // Buffer.
    float *Buffer_p_;

    // Allocate/deallocate memory.
    bool Allocate_Buffer();
    void Deallocate_Buffer();
};

// Print information.
ostream &operator<<(ostream &os,
                    const Iface *p);

} }

#endif

