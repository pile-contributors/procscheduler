/**
 * @file procfactory_inmem.cc
 * @brief Definitions for IVarDef class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procfactory_inmem.h"
#include "../procscheduler-private.h"
#include "procinvocation_inmem.h"
#include "procjob_inmem.h"
#include "assert.h"

/**
 * @class ProcFactory
 *
 */
ProcFactory ProcFactory::instance_;

/* ------------------------------------------------------------------------- */
IProcInvok * ProcFactory::createProcInvok (
        IProcJob *parent, const QString &program,
        const QStringList &arguments, const QStringList &inputs)
{
    return new ProcInvok (parent, program, arguments, inputs);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcJob * ProcFactory::createProcJob (
        ProcScheduler *parent, const QString &name)
{
    return new ProcJob (parent, name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcFactory * ProcFactory::instance ()
{
    return &instance_;
}
/* ========================================================================= */

