/**
 * @file procjob_interface.cc
 * @brief Definitions for IProcJob class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procjob_interface.h"
#include "procinvocation_interface.h"
#include "procscheduler-private.h"
#include <QStringList>

/**
 * @class IProcJob
 *
 */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcJob::IProcJob (ProcScheduler *mng) :
    IProcBase (mng)
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcJob::~IProcJob()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void IProcJob::associateInvocation (IProcInvok *invok, bool b_set)
{
    PROCSCHEDULER_TRACE_ENTRY;
    if (b_set)
        invok->setJob (this);
    else
        invok->setJob (NULL);
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcInvok * IProcJob::firstNonCompleted ()
{
    foreach(IProcBase * iter, kids ()) {
        IProcInvok * invok = iter->toInvok();
        if (invok->execState() == StartingStatus)
            return invok;
    }
    return NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * Default implementation iterates all kids() as provided by the
 * implementation and checks if all of them completed their execution.
 * If so, and any of them failed the status is fail, otherwise
 * the status is OK.
 */
void IProcJob::kidInvokDone (bool /* b_ok */)
{
    PROCSCHEDULER_TRACE_ENTRY;
    for (;;) {

        if (execState () != ExecutingStatus)
            break;

        bool b_done_ok = true;
        foreach(IProcBase * iter, kids ()) {
            switch (iter->execState ()) {
                case CompletedFailStatus:
                    b_done_ok = false;
                    break;
                case CompletedOkStatus:
                    break;
                default:
                    PROCSCHEDULER_TRACE_EXIT;
                    return;
            }
        }

        if (b_done_ok) {
            setExecState (CompletedOkStatus);
        } else {
            setExecState (CompletedFailStatus);
        }
        emit done (execState () == CompletedOkStatus);

        break;
    }
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void IProcJob::execute ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {
        if (!setExecState (StartingStatus))
            break;

        // Connect with the slots of each child.
        foreach(IProcBase * iter, kids ()) {
            IProcInvok * invok = iter->toInvok ();
            connect(invok, &IProcInvok::done,
                    this, &IProcJob::kidInvokDone);
            invok->setExecState (StartingStatus);
        }

        if (!setExecState (ExecutingStatus))
            break;

        // inform them that they may start
        foreach(IProcBase * iter, kids ()) {
            IProcInvok * invok = iter->toInvok ();
            invok->dependencyDone ();
        }
        b_ret = true;
        break;
    }
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */
