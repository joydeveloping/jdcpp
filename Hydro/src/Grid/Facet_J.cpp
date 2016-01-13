/**
 * \file
 * \brief Facet in J direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_J.h"
#include "Iface.h"

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

/**
 * \brief Index linearization.
 *
 * \param[in] i - i index
 * \param[in] k - k index
 *
 * \return
 * Linear index.
 */
int Facet_J::L(int i, int k) const
{
    int lin = i * K_Size() + k;

    assert(lin < Size());

    return lin;
}

/**
 * \brief Set interface.
 *
 * \param[in] i_p - interface pointer
 */
void Facet_J::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int i = i_p->I0(); i < i_p->I1(); i++)
    {
        for (int k = i_p->K0(); k < i_p->K1(); k++)
        {
            Set_Border(L(i, k), i_p);
        }
    }
}

} }

