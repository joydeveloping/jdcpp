/**
 * \file
 * \brief Facet in K direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_K.h"
#include "Iface.h"

namespace Hydro { namespace Grid {


/*
 * Constructors/descturcors.
 */

/**
 * \brief Default constructor.
 *
 * \param[in] i_size - size in I direction
 * \param[in] j_size - size in J direction
 */
Facet_K::Facet_K(int i_size,
                 int j_size)
    : I_Size_(i_size),
      J_Size_(j_size)
{
    Allocate_Memory();
}

/**
 * \brief Default destructor.
 */
Facet_K::~Facet_K()
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
void Facet_K::Print(ostream &os) const
{
    os << "    Facet K:" << endl;

    for (int i = 0; i < I_Size(); i++)
    {
        os << "      ";
        for (int j = 0; j < J_Size(); j++)
        {
            os << Symbol(L(i, j));
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
                    const Facet_K *p)
{
    p->Print(os);
}

/**
 * \brief Index linearization.
 *
 * \param[in] i - i index
 * \param[in] j - j index
 *
 * \return
 * Linear index.
 */
int Facet_K::L(int i, int j) const
{
    int lin = i * J_Size() + j;

    assert(lin < Size());

    return lin;
}

/**
 * \brief Set interface.
 *
 * \param[in] i_p - interface pointer
 */
void Facet_K::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int i = i_p->I0(); i < i_p->I1(); i++)
    {
        for (int j = i_p->J0(); j < i_p->J1(); j++)
        {
            Set_Border(L(i, j), i_p);
        }
    }
}

} }

