/**
 * \file
 * \brief Interface (between blocks) description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_IFACE_H
#define HYDRO_GRID_IFACE_H

#include "Block.h"
#include "Border.h"
#include "configure.h"

namespace Hydro { namespace Grid {

/**
 * \brief Interface class.
 */
class Iface : public Border
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
    int Direction() const { return Direction_; }
    bool Is_NActive() const { return NB()->Is_Active(); }
    bool Is_Active() const { return Is_BActive() || Is_NActive(); }
    bool Is_MPI() const;
    int Cells_Count() const;
    int Buffer_Cells_Count() const;
    int Buffer_Doubles_Count() const { return Buffer_Cells_Count()
                                              * HYDRO_GRID_DYNAMIC_DOUBLES_PER_CELL; }
    int Buffer_Bytes_Count() const { return Buffer_Doubles_Count() * sizeof(double); }
    void *MPI_Buffer() { return static_cast<void *>(Buffer_p_); }

    // From parent.
    bool Is_Iface() const { return true; }
    bool Is_BCond() const { return false; }

    // Additional functions.
    void Set_Buffer_Value(double v);
    bool Check_Buffer_Value(double v,
                            double eps);

private:

    // Identifier.
    int Id_;

    // Pointer to self block.
    Block *B_p_;

    // Coordinated of block interface area.
    int I0_, I1_, J0_, J1_, K0_, K1_;

    // Pointer to neighbour block.
    Block *NB_p_;

    // Direction to neighbour.
    int Direction_;

    // Buffer.
    double *Buffer_p_;

    // Allocate/deallocate memory.
    bool Allocate_Buffer();
    void Deallocate_Buffer();

    // Other.
    void Set_Direction();
};

// Print information.
ostream &operator<<(ostream &os,
                    const Iface *p);

} }

#endif

