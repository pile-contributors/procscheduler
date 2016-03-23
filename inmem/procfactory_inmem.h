/**
 * @file procfactory_inmem.h
 * @brief Declarations for IVarCtx class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_PROC_FACTORY_INMEM_INCLUDE
#define GUARD_PROC_FACTORY_INMEM_INCLUDE

#include <procscheduler/procscheduler-config.h>
#include <procscheduler/procinvocation_interface.h>
#include <procscheduler/procjob_interface.h>
#include <QString>
#include <QList>


//! Factory that creates basic instances.
class PROCSCHEDULER_EXPORT ProcFactory :
        public IProcJobFactory, public IProcInvokFactory {

public:

    //! Creates a variable definition.
    virtual IProcInvok *
    createProcInvok (
            IProcJob * parent,
            const QString & program = QString (),
            const QStringList & arguments = QStringList (),
            const QStringList & inputs = QStringList ());

    //! Creates a job.
    virtual IProcJob *
    createProcJob (
            ProcScheduler * parent,
            const QString & name);

    //! Get the one and only instance.
    static ProcFactory *
    instance ();

private:
    static ProcFactory instance_; /**< the one and only instance */
};

#endif // GUARD_PROC_FACTORY_INMEM_INCLUDE
