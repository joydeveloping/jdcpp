/**
 * \file
 * \brief Facet in I direction realization.
 *
 * \author Alexey Rybakov
 */

#include "Facet_I.h"
#include "Iface.h"

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

/**
 * \brief Index linearization.
 *
 * \param[in] j - j index
 * \param[in] k - k index
 *
 * \return
 * Linear index.
 */
int Facet_I::L(int j, int k) const
{
    int lin = j * K_Size() + k;

    assert(lin < Size());

    return lin;
}

/**
 * \brief Set interface.
 *
 * \param[in] i_p - interface pointer
 */
void Facet_I::Set_Iface(Iface *i_p)
{
    // NB! Interface coordinates - points, so end condition of cycle is "<" (not "<=").
    for (int j = i_p->J0(); j < i_p->J1(); j++)
    {
        for (int k = i_p->K0(); k < i_p->K1(); k++)
        {
            Set_Border(L(j, k), i_p);
        }
    }
}

} }

