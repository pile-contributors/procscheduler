/**
 * @file procjob_inmem.h
 * @brief Declarations for IVarCtx class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROCJOB_INMEM_INCLUDE
#define GUARD_PROCJOB_INMEM_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procjob_interface.h>
#include <QString>
#include <QList>


//! An evaluation context that may contain a number of variables and their values.
class PROCSCHEDULER_EXPORT ProcJob : public IProcJob {

public:

    //! Default constructor.
    ProcJob (
            ProcScheduler * mng,
            const QString & name);

    //! Destructor.
    virtual ~ProcJob ();

    //!

    /* == == == == == == == == == == == == == == == == == */
    /** @name IProcJob interface
     */
    ///@{
public:

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

    //! The list of kids.
    virtual QList<IProcBase *>
    kids () const {
        return invoks_;
    }

    //! A kid at a particular index.
    virtual IProcBase *
    kids (
            int idx) const {
        return invoks_.at (idx);
    }

    //! Number of kids in this item.
    virtual int
    kidsCount () const {
        return invoks_.count ();
    }

    //! Tell if this instance is a node or a leaf.
    virtual bool
    isNode () const {
        return invoks_.count () == 0;
    }

    //! Tell if this instance is at the top level or is part of a node.
    virtual bool
    isVarTopLevel () const {
        return true;
    }

    //! The index of an item inside this node.
    virtual int
    kidIndex (
            IProcBase * pdef) const {
        return invoks_.indexOf (pdef);
    }

    //! Tell if an item is part of this node.
    virtual bool
    isKid (
            IProcBase * pdef) const {
        return invoks_.indexOf (pdef) != -1;
    }

    //! Insert an item inside this node.
    virtual bool
    insertKid (
            int position,
            IProcBase * pdef);

    //! Remove an item from inside this node and delete it.
    virtual bool
    removeKid (
            int position = -1,
            IProcBase * pdef = NULL);

    //! Remove an item inside this node and return it to the caller.
    virtual IProcBase *
    takeKid (
            int position = -1,
            IProcBase * pdef = NULL);

    ///@}
    /* == == == == == == == == == == == == == == == == == */

private:
    QString name_;
    QList<IProcBase *> invoks_;
    Status sts_;
};

#endif // GUARD_PROCJOB_INMEM_INCLUDE
