/**
 * \file
 * \brief 3D vector realization.
 *
 * \author Alexey Rybakov
 */

#include "Vector_3D.h"

namespace Lib { namespace Math {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Vector_3D::Vector_3D()
    : X(0.0), Y(0.0), Z(0.0)
{
}

/**
 * \brief Direct constructor.
 *
 * \param[in] x - x coordinate
 * \param[in] y - y coordinate
 * \param[in] z - z coordinate
 */
Vector_3D::Vector_3D(double x,
                     double y,
                     double z)
    : X(x),
      Y(y),
      Z(z)
{
}

/**
 * \brief Copy constructor.
 *
 * \param[in] v - vector.
 */
Vector_3D::Vector_3D(const Vector_3D &v)
    : X(v.X),
      Y(v.Y),
      Z(v.Z)
{
}

/**
 * \brief Setter.
 *
 * \param[in] x - x component
 * \param[in] y - y component
 * \param[in] z - z component
 */
void Vector_3D::Set(double x,
                    double y,
                    double z)
{
    X = x;
    Y = y;
    Z = z;
}

} }

