/**
 * \file
 * \brief Facet in I direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_I.h"

namespace Hydro { namespace Grid {


/*
 * Constructors/descturcors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] j_size - size in J direction
 * \param[in] k_size - size in K direction
 */
Facet_I::Facet_I(int j_size,
                 int k_size)
    : J_Size_(j_size),
      K_Size_(k_size)
{
    Allocate_Memory();
}

/**
 * \brief Default destructor.
 */
Facet_I::~Facet_I()
{
    Deallocate_Memory();
}

/*
 * Information.
 */

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 */
void Facet_I::Print(ostream &os) const
{
    os << "    Facet I:" << endl;

    for (int j = 0; j < J_Size(); j++)
    {
        os << "      ";
        for (int k = 0; k < K_Size(); k++)
        {
            os << Symbol(L(j, k));
        }
        os << endl;
    }
}

/**
 * \brief Print information.
 *
 * \param[in] os - stream
 * \param[in] p - facet pointer
 */
ostream &operator<<(ostream &os,
                    const Facet_I *p)
{
    p->Print(os);
}

} }

