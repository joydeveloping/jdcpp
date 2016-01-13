/**
 * \file
 * \brief Block border.
 *
 * \author Alexey Rybakov
 */

#ifndef HYDRO_GRID_BORDER_H
#define HYDRO_GRID_BORDER_H

namespace Hydro { namespace Grid {

/**
 * \brief Block border.
 *
 * Block border can be one of two types:
 * 1. Interface between two blocks.
 * 2. Coundary condition (end of calculation area).
 */
class Border
{

public:

    // Constructors/destructors.
    virtual ~Border() {}

    // Characteristics.
    virtual bool Is_Iface() const = 0;
    virtual bool Is_BCond() const = 0;

private:

};

} }

#endif

