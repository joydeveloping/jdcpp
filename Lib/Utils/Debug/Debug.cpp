/**
 * \file
 * \brief Debug realization.
 */

#include <iostream>
#include <cstdlib>

#include "Debug.h"

using std::cout;
using std::endl;

#ifdef DEBUG

namespace Lib { namespace Utils { namespace Debug {

/**
 * \brief Error message and exit.
 *
 * \param[in] str - error string
 * \param[in] file - file name
 * \param[in] line - line number in file
 */
void Debug::Error(string str,
                  string file,
                  int line)
{
    cout << "ERROR : " << str << "! ("
         << file << ", " << line << ")"
         << endl;
    exit(1);
}

/**
 * \brief Conditional error mesage and exit.
 *
 * \param[in] str - string (condition)
 * \param[in] file - file name
 * \param[in] line - line number in file
 */
void Debug::Conditional_Error(string str,
                              string file,
                              int line)
{
    cout << "ERROR : condition \"" << str << "\" failed! ("
         << file << ", " << line << ")"
         << endl;
    exit(1);
}

} } }

#endif

