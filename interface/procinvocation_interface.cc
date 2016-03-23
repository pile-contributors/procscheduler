/**
 * @file procinvocation_interface.cc
 * @brief Definitions for IProcInvok class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procinvocation_interface.h"
#include "procjob_interface.h"
#include "../procscheduler-private.h"
#include "../procscheduler.h"
#include <QStringList>

/**
 * @class IProcInvok
 *
 */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcInvok::IProcInvok (IProcJob *job) :
    IProcBase (job->manager ()),
    job_(job)
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcInvok::~IProcInvok()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void IProcInvok::execute ()
{
    // TODO
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcBase * IProcInvok::parent () const
{
    return job_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool IProcInvok::setParent (IProcBase *value)
{
    if (value->baseType () == JobType) {
        job_ = static_cast<IProcJob*>(value);
        return true;
    } else {
        return false;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * The job may create connections to this slot from the
 * done() signal of objects that need to finish before this one
 * may start. The job also calls this slot when it is started
 * for each of the component invocations.
 *
 * The state of this invocation is asserted to be StartingStatus.
 * The invocation should change this to ExecutingStatus and
 * should add itself to the manager queue if all dependencies are
 * satisfied.
 *
 * Default implementation is designed for a job with linear execution,
 * where each invocation depends on the one directly before it.
 * To that end, if the invocation is the first non-completed
 * invocation in parent job, it is executed.
 */
void IProcInvok::dependencyDone ()
{
    if (this == job()->firstNonCompleted ()) {
        manager ()->addInvokToQueue (this);
    }
}
/* ========================================================================= */
