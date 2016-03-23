/**
 * @file procbase_interface.h
 * @brief Declarations for IProcBase class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCSCHEDULER_INTERFACE_INCLUDE
#define GUARD_PROCSCHEDULER_INTERFACE_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <QString>
#include <QList>
#include <QObject>

class ProcScheduler;
class IProcInvok;
class IProcJob;

//! Interface for base item in the scheduler.
class PROCSCHEDULER_EXPORT IProcBase : public QObject {
    Q_OBJECT
public:

    //! Types of items.
    enum Status {
        PreparingStatus, /**< the instance was created but not run (initial state) */
        StartingStatus, /**< in the process of starting the process */
        EnqueuedStatus, /**< added to a queue before starting */
        ExecutingStatus, /** in process */
        CompletedOkStatus, /**< the run was completed OK */
        CompletedFailStatus /**< the run was completed with an error */
    };

    //! State of the item.
    enum BaseType {
        InvocationType, /**< an IProcJob instance */
        JobType /**< an IProcInvok instance */
    };

public:

    //! Default constructor.
    IProcBase (
            ProcScheduler * mng);

    //! Destructor.
    virtual ~IProcBase ();

    //! The manager where this instance is rooted.
    ProcScheduler *
    manager () const {
        return mng_;
    }

    //! Append a definition inside this node.
    bool
    appendKid (
            IProcBase * pdef) {
        return insertKid (-1, pdef);
    }

    //! Find a kid definition by its label.
    IProcBase *
    findKid (
            const QString & s_label) const;

    //! Cast to invocation if this is the right kind.
    IProcInvok *
    toInvok ();

    //! Cast to job if this is the right kind.
    IProcJob *
    toJob ();


protected:

    //! Set the manager where this instance is rooted.
    void
    setManager (
            ProcScheduler * mng) {
        mng_ = mng;
    }


    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcBase interface
     */
    ///@{
public:

    //! The kind of this instance.
    virtual BaseType
    baseType () const = 0;

    //! Execute this item.
    virtual void
    execute () = 0;

    //! The state of the instance.
    virtual Status
    execState () const = 0;

    //! Change the state of the instance.
    virtual bool
    setExecState (
            Status /* new_state */) {
        return false;
    }

    //! The user label (short string) of this item.
    virtual QString
    label () const = 0;

    //! Change the label (short string) of the item.
    virtual bool
    setLabel (
            const QString &/*label*/) {
        return false;
    }

    //! The parent item.
    virtual IProcBase *
    parent () const {
        return NULL;
    }

    //! Change the parent.
    virtual bool
    setParent (IProcBase *) {
        return false;
    }

    //! The list of kids.
    virtual QList<IProcBase *>
    kids () const {
        return QList<IProcBase *>();
    }

    //! A kid at a particular index.
    virtual IProcBase *
    kids (
            int idx) const {
        return kids ().at (idx);
    }

    //! Number of kids in this item.
    virtual int
    kidsCount () const {
        return kids ().count ();
    }

    //! Tell if this instance is a node or a leaf.
    virtual bool
    isNode () const {
        return kidsCount () == 0;
    }

    //! Tell if this instance is at the top level or is part of a node.
    virtual bool
    isVarTopLevel () const {
        return parent () == NULL;
    }

    //! The index of an item inside this node.
    virtual int
    kidIndex (
            IProcBase * pdef) const {
        return kids ().indexOf (pdef);
    }

    //! Tell if an item is part of this node.
    virtual bool
    isKid (
            IProcBase * pdef) const {
        return kidIndex (pdef) != -1;
    }

    //! Insert an item inside this node.
    virtual bool
    insertKid (
            int /* position */,
            IProcBase * /* pdef */) {
        return false;
    }

    //! Remove an item from inside this node and delete it.
    virtual bool
    removeKid (
            int /* position */ = -1,
            IProcBase * /* pdef */ = NULL) {
        return false;
    }

    //! Remove an item inside this node and return it to the caller.
    virtual IProcBase *
    takeKid (
            int /* position */ = -1,
            IProcBase * /* pdef */ = NULL) {
        return NULL;
    }

public slots:

    //! Informed that the state of a dependency has changed to completed.
    virtual void
    dependencyDone () = 0;

    ///@}
    /* == == == == == == == == == == == == == == == == == */

signals:

    //! The item reached the final status.
    void
    done (
            bool state_ok);

private:
    ProcScheduler * mng_;
}; // class IProcBase


#endif // GUARD_PROCSCHEDULER_INTERFACE_INCLUDE
