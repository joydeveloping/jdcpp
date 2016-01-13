/**
 * \file
 * \brief Facet in J direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_J.h"

namespace Hydro { namespace Grid {


/*
 * Constructors/descturcors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] i_size - size in I direction
 * \param[in] k_size - size in K direction
 */
Facet_J::Facet_J(int i_size,
                 int k_size)
    : I_Size_(i_size),
      K_Size_(k_size)
{
    Allocate_Memory();
}

/**
 * \brief Default destructor.
 */
Facet_J::~Facet_J()
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
void Facet_J::Print(ostream &os) const
{
    os << "    Facet J:" << endl;

    for (int i = 0; i < I_Size(); i++)
    {
        os << "      ";
        for (int k = 0; k < K_Size(); k++)
        {
            os << Symbol(L(i, k));
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
                    const Facet_J *p)
{
    p->Print(os);
}

} }

