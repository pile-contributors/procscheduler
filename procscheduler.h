/**
 * @file procscheduler.h
 * @brief Declarations for ProcScheduler class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCSCHEDULER_H_INCLUDE
#define GUARD_PROCSCHEDULER_H_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <QObject>
#include <QList>
#include <QQueue>

class IProcInvokFactory;
class IProcJobFactory;
class IProcInvok;
class IProcJob;

//! The main management structure for process scheduling.
class PROCSCHEDULER_EXPORT ProcScheduler : public QObject {
    Q_OBJECT
public:

    //! Default constructor.
    ProcScheduler (
            QObject * parent = NULL);

    //! Destructor.
    virtual ~ProcScheduler ();

    //! Creates a process invocation.
    IProcInvok *
    createProcInvok (
            IProcJob * parent,
            const QString & program = QString (),
            const QStringList & arguments = QStringList (),
            const QStringList & inputs = QStringList ());

    //! Creates a job.
    IProcJob *
    createProcJob (
            const QString & name);


    //! Number of kids in this node (leafs return 0).
    int
    jobCount () const {
        return jobs_.count ();
    }

    //! Get the job at a particular index.
    IProcJob *
    job (
            int index) const;

    //! Tell if an integer is a valid job index in this manager.
    bool
    isJobIndexValid (
            int index) const;

    //! Insert a definition inside this manager.
    bool
    insertJob (
            int position,
            IProcJob * pdef);

    //! Insert a definition inside this manager.
    bool
    appendJob (
            IProcJob * pdef) {
        return insertJob (-1, pdef);
    }

    //! Remove a definition from inside this node and delete it.
    bool
    removeJob (
            int position = -1,
            IProcJob * pdef = NULL);

    //! Remove a definition inside this node and return it to the caller.
    IProcJob *
    takeJob (
            int position = -1,
            IProcJob * pdef = NULL);

    //! Move a job to a particular index.
    bool
    setJobIndex (
            int position,
            IProcJob * pdef);

    //! Set the number of concurrent processes to run.
    bool
    setConcurentCount (
            int value);

    //! The number of concurrent processes to run.
    int
    concurentCount () const {
        return concurent_runs_;
    }

    //! Append a program invocation to internal queue.
    bool
    addInvokToQueue (
            IProcInvok * invok);

    //! Run all jobs inside an event loop and only return when all of them are done.
    void
    waitForAll ();

protected:

    //! Takes program invocations from queue and runs them.
    void
    runFromQueue();

    //! The invocation was finished and the queue is updated.
    void
    ackInvokDone (
            IProcInvok *invok);

private slots:

    //! Informed here about a program that exited.
    void
    invokDone ();

signals:

    //! A job was created by this manager.
    void
    jobCreated (
            IProcJob * job);

    //! A program invocation was created by this manager.
    void
    invokCreated (
            IProcInvok * job);

    //! A program invocation has started.
    void
    invokStarted (
            IProcInvok * job);

    //! A program invocation has finished.
    void
    invokFinished (
            IProcInvok * job);

private:
    IProcInvokFactory * invok_factory_; /**< factory for invocations */
    IProcJobFactory * job_factory_; /**< factory for jobs */
    QList<IProcJob*> jobs_; /**< list of jobs associated with this instance */
    int concurent_runs_; /**< number of concurrent processes */
    QQueue<IProcInvok*> invoks_; /**< the list of program invocations */
    QList<IProcInvok*> running_; /**< the list of programs running*/
};

#endif // GUARD_PROCSCHEDULER_H_INCLUDE
