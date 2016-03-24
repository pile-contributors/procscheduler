/**
 * @file procjob_inmem.cc
 * @brief Definitions for IVarDef class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procjob_inmem.h"
#include "../procscheduler-private.h"
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

/* ------------------------------------------------------------------------- */
bool ProcJob::insertKid (int position, IProcBase *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    if (invoks_.contains (pdef))
        return false;

    if ((position == -1) || (position >= invoks_.count ())) {
        invoks_.append (pdef);
    } else {
        invoks_.insert (position, pdef);
    }

    PROCSCHEDULER_TRACE_EXIT;
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcJob::removeKid (int position, IProcBase *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    IProcBase * ivd = takeKid (position, pdef);
    if (ivd == NULL)
        return false;
    delete ivd;
    PROCSCHEDULER_TRACE_EXIT;
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcBase *ProcJob::takeKid (int position, IProcBase *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    if (position < 0) {
        if (pdef == NULL)
            return NULL;
        position = invoks_.indexOf (pdef);
        if (position == -1)
            return NULL;
    } else if (position >= invoks_.count ()) {
        return NULL;
    } else if (pdef == NULL) {
        pdef = invoks_.at (position);
    } else {
        assert(invoks_.at (position) == pdef);
    }

    invoks_.removeAt (position);
    PROCSCHEDULER_TRACE_EXIT;
    return pdef;
}
/* ========================================================================= */
