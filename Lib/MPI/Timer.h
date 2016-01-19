/**
 * \file
 * \brief MPI timer description.
 *
 * \author Alexey Rybakov
 */

#ifndef LIB_MPI_TIMER_H
#define LIB_MPI_TIMER_H

namespace Lib { namespace MPI {

/**
 * \brief MPI timer.
 */
class Timer
{

public:

    // Constructors/destructors.
    Timer();
    ~Timer();

    // General commands.
    double Init();
    double Start();
    double Stop();
    double Time();

private:

    // Total time value.
    double Total_;

    // Run flag (is time is computed).
    bool Is_Active_;

    // Start point.
    double Last_Start_;

    // Get last time if timer is active.
    double Last_Time();
};

} }

#endif

