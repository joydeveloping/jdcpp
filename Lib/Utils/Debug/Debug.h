/**
 * \file
 * \brief Debug class description.
 *
 * All functions in this module work with debug option -DDEBUG,
 * differently they all are empty stubs.
 */

#ifndef LIB_UTILS_DEBUG_DEBUG_H
#define LIB_UTILS_DEBUG_DEBUG_H

#include <string>

using std::string;

/**
 * \brief Error message and exit.
 */
#ifdef DEBUG
#define DEBUG_ERROR(str) Debug::Error(str, __FILE__, __LINE__)
#else
#define DEBUG_ERROR(str)
#endif

/**
 * \brief Not implemented error.
 */
#ifdef DEBUG
#define DEBUG_NOT_IMPLEMENTED() DEBUG_ERROR("not implemented")
#else
#define DEBUG_NOT_IMPLEMENTED()
#endif

/**
 * \brief Check condition.
 */
#ifdef DEBUG
#define DEBUG_CHECK(cond) \
    if (!(cond)) \
    { \
        Debug::Conditional_Error(#cond, __FILE__, __LINE__); \
    }
#else
#define DEBUG_CHECK(cond)
#endif

namespace Lib { namespace Utils { namespace Debug {

#ifdef DEBUG

/**
 * \brief Class for debug.
 */
class Debug
{

public:

    static void Error(string str,
                      string file,
                      int line);
    static void Conditional_Error(string str,
                                  string file,
                                  int line);
};

#endif

} } }

#endif

