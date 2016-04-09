/**
 * @file procjob_interface.h
 * @brief Declarations for IVarDef class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCJOB_INTERFACE_INCLUDE
#define GUARD_PROCJOB_INTERFACE_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procbase_interface.h>
#include <QString>
#include <QList>

class IProcJobFactory;
class IProcInvok;

//! Interface for a job that groups togheter one or more program invocations.
class PROCSCHEDULER_EXPORT IProcJob : public IProcBase {
    Q_OBJECT
public:

    //! Default constructor.
    IProcJob (
            ProcScheduler * mng);

    //! Destructor.
    virtual ~IProcJob ();

protected:

    //! Associate an invocation with this job.
    void
    associateInvocation (
            IProcInvok * invok,
            bool b_set);

    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcJob interface
     */
    ///@{
public:

    //! The kind of this instance.
    virtual BaseType
    baseType () const {
        return JobType;
    }

    //! Informed that the state of a dependency has changed to completed.
    virtual void
    dependencyDone () {}

    //! First non-completed child invocation.
    virtual IProcInvok *
    firstNonCompleted ();

    //! Execute this item.
    void
    execute ();

    ///@}
    /* == == == == == == == == == == == == == == == == == */

protected slots:

    //! May be used to connect child invocations; changes the state when all completed.
    virtual void
    kidInvokDone (
            bool b_ok);

private:

}; // class IProcJob


//! Factory class for creating process invocations.
class PROCSCHEDULER_EXPORT IProcJobFactory {
public:

    //! Creates a variable definition.
    virtual IProcJob *
    createProcJob (
            ProcScheduler * parent,
            const QString & name) = 0;

}; // class IProcJobFactory


#endif // GUARD_PROCJOB_INTERFACE_INCLUDE
