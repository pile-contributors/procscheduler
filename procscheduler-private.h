/**
 * @file procscheduler-private.h
 * @brief Declarations for ProcScheduler class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCSCHEDULER_PRIVATE_H_INCLUDE
#define GUARD_PROCSCHEDULER_PRIVATE_H_INCLUDE

#include <procscheduler/procscheduler-config.h>

#if 0
#    define PROCSCHEDULER_DEBUGM printf
#else
#    define PROCSCHEDULER_DEBUGM black_hole
#endif

#if 0
#    define PROCSCHEDULER_TRACE_ENTRY printf("PROCSCHEDULER ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define PROCSCHEDULER_TRACE_ENTRY
#endif

#if 0
#    define PROCSCHEDULER_TRACE_EXIT printf("PROCSCHEDULER EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define PROCSCHEDULER_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_PROCSCHEDULER_PRIVATE_H_INCLUDE
