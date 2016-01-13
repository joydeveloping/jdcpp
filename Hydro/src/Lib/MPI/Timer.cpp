/**
 * \file
 * \brief MPI timer realization.
 *
 * \author Alexey Rybakov
 */

#include <cassert>
#include <mpi.h>
#include "Timer.h"

namespace Lib { namespace MPI {

/*
 * Constructors/destructors.
 */

/**
 * \brief Default constructor.
 */
Timer::Timer()
    : Total_(0.0),
      Is_Active_(false),
      Last_Start_(0.0)
{
}

/**
 * \brief Default destructor.
 */
Timer::~Timer()
{
}

/*
 * General commands.
 */

/**
 * \brief Initialization.
 *
 * \return
 * Total time value.
 */
double Timer::Init()
{
    Total_ = 0.0;
    Is_Active_ = 0.0;
    Last_Start_ = 0.0;

    return 0.0;
}

/**
 * \brief Start timer.
 *
 * \return
 * Total tmie value.
 */
double Timer::Start()
{
    if (!Is_Active_)
    {
        Last_Start_ = MPI_Wtime();
        Is_Active_ = true;
    }

    return Time();
}

/**
 * \brief Stop timer.
 *
 * \return
 * Total time value.
 */
double Timer::Stop()
{
    if (Is_Active_)
    {
        Total_ += Last_Time();
        Is_Active_ = false;
    }

    return Time();
}

/**
 * \brief Get current time value.
 *
 * \return
 * Total time value.
 */
double Timer::Time()
{
    double r = Total_;

    if (Is_Active_)
    {
        r += Last_Time();
    }

    return r;
}

/**
 * \brief Get last time is timer is active.
 *
 * \return
 * Last time value.
 */
double Timer::Last_Time()
{
    assert(Is_Active_);

    return MPI_Wtime() - Last_Start_;
}

} }

