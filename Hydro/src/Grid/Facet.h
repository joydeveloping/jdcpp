/**
 * \file
 * \brief Facet description.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_FACET_H
#define HYDRO_GRID_FACET_H

#include <cassert>
#include "Lib/IO/io.h"
#include "Border.h"

namespace Hydro { namespace Grid {

class Iface;

/**
 * \brief Facet.
 */
class Facet
{

public:

    // Constructors/destructors.
    Facet(int height,
          int width);
    virtual ~Facet();

    // Simple data and characteristics.
    int Height() const { return Height_; }
    int Width() const { return Width_; }
    virtual bool Is_Direction_I() const = 0;
    virtual bool Is_Direction_J() const = 0;
    virtual bool Is_Direction_K() const = 0;
    int Size() const { return Width() * Height(); }

    // Information.
    char Symbol(int bi) const;
    void Print(ostream &os) const;

    // Set interface.
    virtual void Set_Iface(Iface *i_p) = 0;

private:

    /*
     * Sizes.
     *
     *      Width
     *   *---------*
     *   |         |
     *   |         | Height
     *   |         |
     *   *---------*
     */
    int Height_, Width_;

    // Border pointers.
    Border **Borders_p_;

    // Memory managements.
    void Allocate_Memory();
    void Deallocate_Memory();

    // Set borders.
    void Set_Border(int i, Border *p) { Borders_p_[i] = p; }

    // Linearization.
    int L(int i, int j) const;

protected:

    // Set border.
    void Set_Border(int i, int j, Border *p) { Set_Border(L(i, j), p); }
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet *p);

} }

#endif

