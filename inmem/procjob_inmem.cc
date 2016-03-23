/**
 * @file procjob_inmem.cc
 * @brief Definitions for IVarDef class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procjob_inmem.h"
#include "procscheduler-private.h"
#include "assert.h"

/**
 * @class ProcJob
 *
 */

/* ------------------------------------------------------------------------- */
/**
 */
ProcJob::ProcJob (ProcScheduler * mng, const QString & name) :
    IProcJob(mng),
    name_(name),
    invoks_(),
    sts_(PreparingStatus)
{
    PROCSCHEDULER_TRACE_ENTRY;
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 */
ProcJob::~ProcJob ()
{
    PROCSCHEDULER_TRACE_ENTRY;

    qDeleteAll (invoks_);
    invoks_.clear ();

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */
