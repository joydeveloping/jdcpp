/**
 * \file
 * \brief Main module (enter point).
 *
 * \author Alexey Rybakov
 */

#include "Lib/IO/io.h"
#include "Grid/Grid.h"

/**
 * \brief Main function (enter point).
 *
 * \return
 * Run status.
 */
int main()
{
    Hydro::Grid::Grid *grid_p = new Hydro::Grid::Grid(5);

    cout << grid_p;

    delete grid_p;

    return 0;
}

