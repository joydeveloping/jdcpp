/**
 * \file
 * \brief Fluid Dynamic Parameters realization.
 *
 * \author Alexey Rybakov
 */

#include "Fluid_Dyn_Pars.h"

namespace Hydro { namespace Grid {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars()
    : R(0.0),
      V(),
      E(0.0),
      P(0.0)
{
}

/**
 * Copy constructor.
 *
 * \param[in] fdp - copy source.
 */
Fluid_Dyn_Pars::Fluid_Dyn_Pars(const Fluid_Dyn_Pars &fdp)
{
    R = fdp.R;
    V = fdp.V;
    E = fdp.E;
    P = fdp.P;
}

/*
 * Forms: normal <-> expand.
 */

/**
 * \brief Convert normal form to expand.
 */
void Fluid_Dyn_Pars::Normal_To_Expand()
{
    E = R * (E + 0.5 * V.Mod_2());
    V *= R;
}

/**
 * \brief Convert expand form to normal.
 */
void Fluid_Dyn_Pars::Expand_To_Normal()
{
    V /= R;
    E = E / R - 0.5 * V.Mod_2();

    // TODO:
    // Remove.
    P = (1.4 - 1.0) * R * E;
}

/*
 * Flow.
 */

/**
 * \brief Through X edge.
 *
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_X(double dr,
                            double dv,
                            double de)
{
    R -= dr;
    V.X -= dv;
    E -= de;
}

/**
 * \brief Through Y edge.
 *
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_Y(double dr,
                            double dv,
                            double de)
{
    R -= dr;
    V.Y -= dv;
    E -= de;
}

/**
 * \brief Through Z edge.
 *
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_Z(double dr,
                            double dv,
                            double de)
{
    R -= dr;
    V.Z -= dv;
    E -= de;
}

/**
 * \brief Through Z edge.
 *
 * \param[in,out] fdp - neighbour fdp
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_X(Fluid_Dyn_Pars &fdp,
                            double dr,
                            double dv,
                            double de)
{
    Flow_X(dr, dv, de);
    fdp.Flow_X(-dr, -dv, -de);
}

/**
 * \brief Through Z edge.
 *
 * \param[in,out] fdp - neighbour fdp
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_Y(Fluid_Dyn_Pars &fdp,
                            double dr,
                            double dv,
                            double de)
{
    Flow_Y(dr, dv, de);
    fdp.Flow_Y(-dr, -dv, -de);
}

/**
 * \brief Through Z edge.
 *
 * \param[in,out] fdp - neighbour fdp
 * \param[in] dr - rho flow
 * \param[in] dv - v flow
 * \param[in] de - e flow
 */
void Fluid_Dyn_Pars::Flow_Z(Fluid_Dyn_Pars &fdp,
                            double dr,
                            double dv,
                            double de)
{
    Flow_Z(dr, dv, de);
    fdp.Flow_Z(-dr, -dv, -de);
}

/*
 * Print information.
 */

/**
 * \brief Print info.
 *
 * \param[in] os - stream
 * \param[in] p - parameters to print
 *
 * \return
 * Stream.
 */
ostream &operator<<(ostream &os,
                    const Fluid_Dyn_Pars *p)
{
    os << "R = " << setw(10) << p->R
       << " Vx = " << setw(10) << p->V.X
       << " Vy = " << setw(10) << p->V.Y
       << " Vz = " << setw(10) << p->V.Z
       << " E = " << setw(10) << p->E
       << " P = " << setw(10) << p->P << endl;
}

} }

