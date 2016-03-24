/**
 * @file varctx_interface.h
 * @brief Declarations for IVarCtx class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROC_INVOK_INMEM_INCLUDE
#define GUARD_PROC_INVOK_INMEM_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procinvocation_proc.h>
#include <QString>
#include <QList>
#include <QProcess>


//! An in-memory invocation.
class PROCSCHEDULER_EXPORT ProcInvok : public ProcInvokProc {

public:

    //! Default constructor.
    ProcInvok (
            IProcJob * job,
            const QString & program,
            const QStringList & arguments = QStringList (),
            const QStringList & inputs = QStringList ());

    //! Destructor.
    virtual ~ProcInvok ();


    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcInvok interface
     */
    ///@{
public:

    //! The user label (short string) of this item.
    virtual QString
    label () const {
        return name_;
    }

    //! Change the label (short string) of the item.
    virtual bool
    setLabel (
            const QString & label) {
        name_ = label;
        return true;
    }

    //! The state of the instance.
    virtual Status
    execState () const {
        return sts_;
    }

    //! Change the state of the instance.
    virtual bool
    setExecState (
            Status new_state) {
        sts_ = new_state;
        return true;
    }

    ///@}
    /* == == == == == == == == == == == == == == == == == */


private:
    QString name_;
    Status sts_;
};

#endif // GUARD_PROC_INVOK_INMEM_INCLUDE
