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
    Facet();
    virtual ~Facet();

    // Simple data and characteristics.
    virtual bool Is_Direction_I() const = 0;
    virtual bool Is_Direction_J() const = 0;
    virtual bool Is_Direction_K() const = 0;
    virtual int Size() const = 0;

    // Information.
    char Symbol(int bi) const;
    virtual void Print(ostream &os) const = 0;

    // Set interface.
    virtual void Set_Iface(Iface *i_p) = 0;

private:

    // Border pointers.
    Border **Borders_p_;

    // Linearization of index.
    virtual int L(int, int) const = 0;

protected:

    // Memory managements.
    void Allocate_Memory();
    void Deallocate_Memory();

    // Set borders.
    void Set_Border(int i, Border *p) { Borders_p_[i] = p; }
};

// Print information.
ostream &operator<<(ostream &os,
                    const Facet *p);

} }

#endif

