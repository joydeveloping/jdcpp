/**
 * \file
 * \brief 3D vector.
 *
 * \author Alexey Rybakov
 */

#ifndef LIB_MATH_VECTOR_3D_H
#define LIB_MATH_VECTOR_3D_H

namespace Lib { namespace Math {

/**
 * \brief 3D vector class.
 */
class Vector_3D
{

public:

    // Data (not private because there is no limitations on vector components.
    double X, Y, Z;

    // Constructors/destructors.
    Vector_3D();
    Vector_3D(double x,
              double y,
              double z);
    Vector_3D(const Vector_3D &v);

    // Setter/
    void Set(double x,
             double y,
             double z);

    // Overload.
    Vector_3D &operator*=(double d);
    Vector_3D &operator/=(double d);

    // Operations.
    double Mod_2() { return X * X + Y * Y + Z * Z; }
};

} }

#endif

