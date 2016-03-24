/**
 * @file procinvocation_inmem.cc
 * @brief Definitions for IVarDef class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procinvocation_proc.h"
#include "../procscheduler-private.h"
#include "assert.h"
#include <QFileInfo>

/**
 * @class ProcInvok
 *
 */

/* ------------------------------------------------------------------------- */
ProcInvokProc::ProcInvokProc (
        IProcJob * job) :
    IProcInvok (job),
    proc_(),
    start_time_ (),
    end_time_ (),
    inputs_(),
    errors_ (),
    states_ (),
    b_started_ (false)
{
    PROCSCHEDULER_TRACE_ENTRY;

    connect (&proc_, SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(errorSlot(QProcess::ProcessError)));
    connect (&proc_, SIGNAL(finished(int,QProcess::ExitStatus)),
             this, SLOT(finishedSlot(int,QProcess::ExitStatus)));
    connect (&proc_, SIGNAL(readyReadStandardError()),
             this, SLOT(readyReadStandardErrorSlot()));
    connect (&proc_, SIGNAL(readyReadStandardOutput()),
             this, SLOT(readyReadStandardOutputSlot()));
    connect (&proc_, SIGNAL(started()),
             this, SLOT(startedSlot()));
    connect (&proc_, SIGNAL(stateChanged (QProcess::ProcessState)),
             this, SLOT(stateChangedSlot (QProcess::ProcessState)));

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcInvokProc::~ProcInvokProc ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    disconnect (&proc_, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(errorSlot(QProcess::ProcessError)));
    disconnect (&proc_, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(finishedSlot(int,QProcess::ExitStatus)));
    disconnect (&proc_, SIGNAL(readyReadStandardError()),
                this, SLOT(readyReadStandardErrorSlot()));
    disconnect (&proc_, SIGNAL(readyReadStandardOutput()),
                this, SLOT(readyReadStandardOutputSlot()));
    disconnect (&proc_, SIGNAL(started()),
                this, SLOT(startedSlot()));
    disconnect (&proc_, SIGNAL(stateChanged (QProcess::ProcessState)),
                this, SLOT(stateChangedSlot (QProcess::ProcessState)));
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::errorSlot (QProcess::ProcessError error)
{
    PROCSCHEDULER_TRACE_ENTRY;
    errors_.append (error);
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::finishedSlot (int exit_code, QProcess::ExitStatus)
{
    PROCSCHEDULER_TRACE_ENTRY;
    end_time_ = QDateTime::currentDateTime ();
    setExecState (exit_code == 0 ? CompletedOkStatus : CompletedFailStatus);
    emit done (exit_code == 0 ? CompletedOkStatus : CompletedFailStatus);
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::readyReadStandardErrorSlot ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    QString s (proc_.readAllStandardError ());
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::readyReadStandardOutputSlot ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    QString s (proc_.readAllStandardOutput());
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::startedSlot ()
{
    PROCSCHEDULER_TRACE_ENTRY;
    b_started_ = true;
    start_time_ = QDateTime::currentDateTime ();
    setExecState (ExecutingStatus);
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::stateChangedSlot (QProcess::ProcessState newState)
{
    PROCSCHEDULER_TRACE_ENTRY;
    states_.append (newState);
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString ProcInvokProc::program () const
{
    return proc_.program ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QStringList ProcInvokProc::arguments () const
{
    return proc_.arguments ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QStringList ProcInvokProc::inputs () const
{
    return inputs_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString ProcInvokProc::workingDirectory () const
{
    return proc_.workingDirectory ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QProcessEnvironment ProcInvokProc::processEnvironment () const
{
    return proc_.processEnvironment ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QProcess::ProcessError ProcInvokProc::error () const
{
    return proc_.error ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QProcess::ProcessState ProcInvokProc::state () const
{
    return proc_.state ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Q_PID ProcInvokProc::pid () const
{
    return proc_.pid ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
qint64 ProcInvokProc::processId () const
{
    return proc_.processId ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int ProcInvokProc::exitCode () const
{
    return proc_.exitCode ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QProcess::ExitStatus ProcInvokProc::exitStatus () const
{
    return proc_.exitStatus ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::setProgram (const QString & program)
{
    proc_.setProgram (program);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::setArguments (const QStringList & arguments)
{
    proc_.setArguments (arguments);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::setInputs (const QStringList & inputs)
{
    inputs_ = inputs;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::setWorkingDirectory (const QString & dir)
{
    proc_.setWorkingDirectory (dir);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::setProcessEnvironment (
        const QProcessEnvironment & environment)
{
    proc_.setProcessEnvironment (environment);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ProcInvokProc::execute ()
{
    // start the program
    proc_.start (QIODevice::ReadWrite);
    if (!proc_.waitForStarted ()) {
        setExecState (CompletedFailStatus);
        return;
    }

    // provide the input
    foreach (const QString & s, inputs_) {
        proc_.write (s.toLatin1 ().constData ());
    }
    proc_.closeWriteChannel();
}
/* ========================================================================= */
