/**
 * @file procscheduler.cc
 * @brief Definitions for ProcScheduler class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procscheduler.h"
#include "procscheduler-private.h"
#include "procscheduler/procinvocation_interface.h"
#include "procscheduler/procjob_interface.h"
#include "inmem/procfactory_inmem.h"
#include "assert.h"

#include <QDir>
#include <QEventLoop>


/**
 * @class ProcScheduler
 *
 */

/* ------------------------------------------------------------------------- */
/**
 *
 */
ProcScheduler::ProcScheduler (QObject * parent) :
    QObject (parent),
    invok_factory_(ProcFactory::instance ()),
    job_factory_(ProcFactory::instance ()),
    jobs_(),
    concurent_runs_(2),
    invoks_(),
    running_()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 *
 */
ProcScheduler::~ProcScheduler()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcInvok * ProcScheduler::createProcInvok (
        IProcJob *parent, const QString &program,
        const QStringList &arguments, const QStringList &inputs)
{
    IProcInvok * result = invok_factory_->createProcInvok (
                parent, program, arguments, inputs);
    if (result != NULL) {
        emit invokCreated (result);
    }
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcJob * ProcScheduler::createProcJob (
        ProcScheduler *parent, const QString &name)
{
    IProcJob * result = job_factory_->createProcJob (parent, name);
    if (result != NULL) {
        appendJob (result);
        emit jobCreated (result);
    }
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcJob * ProcScheduler::job (int index) const
{
    if (isJobIndexValid (index)) {
        return jobs_.at (index);
    } else {
        return NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::isJobIndexValid(int index) const
{
    return ((index >= 0) && (index < jobs_.count ()));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::insertJob (int position, IProcJob *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    if (jobs_.contains (pdef))
        return false;

    if ((position == -1) || (position >= jobs_.count ())) {
        jobs_.append (pdef);
    } else {
        jobs_.insert (position, pdef);
    }

    PROCSCHEDULER_TRACE_EXIT;
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::removeJob (int position, IProcJob *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    IProcJob * ivd = takeJob (position, pdef);
    if (ivd == NULL)
        return false;
    delete ivd;
    PROCSCHEDULER_TRACE_EXIT;
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcJob * ProcScheduler::takeJob (int position, IProcJob *pdef)
{
    PROCSCHEDULER_TRACE_ENTRY;
    if (position < 0) {
        if (pdef == NULL)
            return NULL;
        position = jobs_.indexOf (pdef);
        if (position == -1)
            return NULL;
    } else if (position >= jobs_.count ()) {
        return NULL;
    } else if (pdef == NULL) {
        pdef = jobs_.at (position);
    } else {
        assert(jobs_.at (position) == pdef);
    }

    jobs_.removeAt (position);
    PROCSCHEDULER_TRACE_EXIT;
    return pdef;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::setJobIndex (int position, IProcJob *pdef)
{
    int idx = jobs_.indexOf (pdef);
    if (idx == -1)
        return false;
    if (position < 0)
        position = jobs_.count ();
    else if (position > jobs_.count ())
        position = jobs_.count ();
    jobs_.move (idx, position);
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::setConcurentCount (int value)
{
    if (value < 1)
        return false;
    if (value > 256)
        return false;
    concurent_runs_ = value;
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool ProcScheduler::addInvokToQueue (IProcInvok *invok)
{
    PROCSCHEDULER_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {
        if (invok->execState () != IProcBase::StartingStatus) {
            PROCSCHEDULER_DEBUGM("Wrong state for invocation: %d\n",
                                 invok->execState ());
            break;
        }

        if (!invok->setExecState (IProcBase::EnqueuedStatus)) {
            PROCSCHEDULER_DEBUGM("Invocation refused to enter queue\n");
            break;
        }

        // Add the item to the list.
        invoks_.enqueue (invok);

        // May even start if empty slots are available.
        runFromQueue ();

        b_ret = true;
        break;
    }
    PROCSCHEDULER_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcScheduler::waitForAll ()
{
    QEventLoop evl;
    connect (this, &ProcScheduler::invokFinished,
             &evl, &QEventLoop::quit);
    while ((!running_.isEmpty ()) || (!invoks_.isEmpty ())) {
        evl.exec();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcScheduler::ackInvokDone (IProcInvok *invok)
{
    PROCSCHEDULER_TRACE_ENTRY;
    for (;;) {

        disconnect (invok, &IProcInvok::done,
                    this, &ProcScheduler::invokDone);

        //Locate and extract from queue.
        if (!running_.contains (invok)) {
            if (invoks_.contains (invok)) {
                invoks_.removeOne (invok);
            } else {
                PROCSCHEDULER_DEBUGM("Sender not part of this instance's queue\n");
                break;
            }
        } else {
            running_.removeOne (invok);
        }

        // Last attempt to keep an consistent status.
        // Should have been changed by the implementation.
        if (invok->execState () == IProcBase::ExecutingStatus) {
            invok->setExecState (IProcBase::CompletedOkStatus);
        }

        emit invokFinished (invok);
        break;
    }
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcScheduler::invokDone ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    for (;;) {
        IProcInvok *invok = qobject_cast<IProcInvok *> (sender());
        if (invok == NULL) {
            PROCSCHEDULER_DEBUGM("Sender to invokDone should be an IProcInvok\n");
            break;
        }

        ackInvokDone (invok);

        runFromQueue ();
        break;
    }
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcScheduler::runFromQueue ()
{
    PROCSCHEDULER_TRACE_ENTRY;

    while (running_.count () < concurent_runs_) {
        if (invoks_.isEmpty())
            break;
        IProcInvok *invok = invoks_.dequeue ();
        invok->setWorkingDirectory (QDir::currentPath ());
        invok->setProcessEnvironment (QProcessEnvironment::systemEnvironment ());
        if (!invok->setExecState (IProcBase::ExecutingStatus)) {
            PROCSCHEDULER_DEBUGM("Invocation refused to start\n");
            invok->setExecState (IProcBase::CompletedFailStatus);
            break;
        }

        running_.append (invok);
        connect(invok, &IProcInvok::done,
                this, &ProcScheduler::invokDone);

        invok->execute ();
        emit invokStarted (invok);
        if (invok->execState() != IProcBase::ExecutingStatus) {
            ackInvokDone (invok);
        }
    }

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */
