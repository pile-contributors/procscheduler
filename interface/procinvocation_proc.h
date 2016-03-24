/**
 * @file procinvocation_proc.h
 * @brief Declarations for ProcInvokProc class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROC_INVOK_PROC_INCLUDE
#define GUARD_PROC_INVOK_PROC_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procinvocation_interface.h>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QProcess>


//! An extends the interface with an embedded QProcess.
class PROCSCHEDULER_EXPORT ProcInvokProc : public IProcInvok {
    Q_OBJECT
public:

    //! Default constructor.
    ProcInvokProc (
            IProcJob * job);

    //! Destructor.
    virtual ~ProcInvokProc ();

    //! Get the duration in seconds.
    qint64
    runDuration() const {
        return end_time_.secsTo (start_time_);
    }

    //! Get the duration in seconds.
    int durationInSeconds () {
        return static_cast<int>(end_time_.secsTo (start_time_));
    }

    //! Get the duration in milliseconds.
    int durationInMiliSeconds () {
        return static_cast<int>(end_time_.msecsTo (start_time_));
    }

    //! The list of errors the process experienced.
    const QList<QProcess::ProcessError>
    errors () const {
        return errors_;
    }

    //! The list of states the process went through.
    const QList<QProcess::ProcessState>
    states () const {
        return states_;
    }

    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcInvok interface
     */
    ///@{
public:

    //! Execute this item.
    virtual void
    execute ();

    ///@}
    /* == == == == == == == == == == == == == == == == == */


    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcInvok interface
     */
    ///@{
public:

    //! .
    virtual QString
    program () const;

    //! .
    virtual QStringList
    arguments () const;

    //! .
    virtual QStringList
    inputs () const;

    //! .
    virtual QString
    workingDirectory () const;

    //! .
    virtual QProcessEnvironment
    processEnvironment () const;

    //! .
    virtual void
    setProgram (
            const QString & program);

    //! .
    virtual void
    setArguments (
            const QStringList & arguments);

    //! .
    virtual void
    setInputs (
            const QStringList & arguments);

    //! .
    virtual void
    setWorkingDirectory (
            const QString &dir);

    //! .
    virtual void
    setProcessEnvironment (
            const QProcessEnvironment &environment);


    ///@}
    /* == == == == == == == == == == == == == == == == == */



    /* == == == == == == == == == == == == == == == == == */
    /** @name Shortcuts for embedded QProcess.
     */
    ///@{
public:

    QProcess  &
    embeddedProcess ();

    QProcess::ProcessError
    error () const;

    QProcess::ProcessState
    state () const;

    Q_PID
    pid () const;

    qint64
    processId () const;

    int
    exitCode () const;

    QProcess::ExitStatus
    exitStatus () const;

    ///@}
    /* == == == == == == == == == == == == == == == == == */


private slots:

    //! Some output coming out of error channel.
    void
    readyReadStandardErrorSlot ();

    //! Some output coming out of output channel.
    void
    readyReadStandardOutputSlot ();

    //! Connected to keep the started/not started state.
    void
    startedSlot ();

    //! Connected to keep the started/not started state.
    void
    finishedSlot (
            int exitCode,
            QProcess::ExitStatus exitStatus);

    //! Track this slot to accumulate the list of states.
    void
    stateChangedSlot (
            QProcess::ProcessState newState);

    //! Accumulate errors here.
    void
    errorSlot (
            QProcess::ProcessError error);

private:
    QProcess proc_;
    QDateTime start_time_; /**< the time when the process was started */
    QDateTime end_time_; /**< the time when the process ended */
    QStringList inputs_; /**< the list of strings to provide to the program */
    QList<QProcess::ProcessError> errors_; /**< list of errors */
    QList<QProcess::ProcessState> states_; /**< list of states*/
    bool b_started_; /**< is the process already running? */
};

#endif // GUARD_PROC_INVOK_PROC_INCLUDE
