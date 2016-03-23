/**
 * @file procinvocation_interface.h
 * @brief Declarations for IVarDef class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCINVOK_INTERFACE_INCLUDE
#define GUARD_PROCINVOK_INTERFACE_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procbase_interface.h>
#include <QString>
#include <QList>

class IProcInvokFactory;
class IProcJob;

//! Interface for an invocation.
class PROCSCHEDULER_EXPORT IProcInvok : public IProcBase {
    Q_OBJECT

    friend class IProcJob;
public:

    //! Default constructor.
    IProcInvok (
            IProcJob * job);

    //! Destructor.
    virtual ~IProcInvok ();

    //! The job where this program invocation belongs.
    IProcJob *
    job () const {
        return job_;
    }

protected:

    //! Set the job where this invocation belongs.
    void
    setJob (
            IProcJob * value) {
        job_ = value;
    }

    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcInvok interface
     */
    ///@{
public:

    //! Execute this item.
    virtual void
    execute ();

    //! The kind of this instance.
    virtual BaseType
    baseType () const {
        return InvocationType;
    }

    //! The parent item.
    virtual IProcBase *
    parent () const;

    //! Change the group where the variable belongs.
    virtual bool
    setParent (
            IProcBase *value);

    //! Informed that the state of a dependency has changed to completed.
    virtual void
    dependencyDone ();

    ///@}
    /* == == == == == == == == == == == == == == == == == */

private:
    IProcJob * job_;
}; // class IProcInvok


//! Factory class for creating process invocations.
class PROCSCHEDULER_EXPORT IProcInvokFactory {
public:

    //! Creates a variable definition.
    virtual IProcInvok *
    createProcInvok (
            IProcJob * parent,
            const QString & program,
            const QStringList & arguments = QStringList (),
            const QStringList & inputs = QStringList ()) = 0;

}; // class IProcInvokFactory


#endif // GUARD_PROCINVOK_INTERFACE_INCLUDE
