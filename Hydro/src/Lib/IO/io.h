/**
 * \file
 * \brief Input-Output declarations.
 *
 * \author Alexey Rybakov
 */

#ifndef LIB_IO_IO_H
#define LIB_IO_IO_H

#include <mpi.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

namespace  Lib { namespace IO {

/**
 * \brief Convert type to string.
 *
 * \param[in] val - value
 *
 * \return
 * String (value converted to string).
 */
template <typename T>
string To_String(T val)
{
    ostringstream oss;

    oss << val;

    return oss.str();
}

} }

#endif

