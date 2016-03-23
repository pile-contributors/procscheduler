/**
 * @file procinvocation_inmem.cc
 * @brief Definitions for IVarDef class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procinvocation_inmem.h"
#include "procscheduler-private.h"
#include "assert.h"
#include <QFileInfo>

/**
 * @class ProcInvok
 *
 */

/* ------------------------------------------------------------------------- */
ProcInvok::ProcInvok (
        IProcJob * job, const QString &program,
        const QStringList &arguments, const QStringList &inputs) :
    IProcInvok (job),
    name_(QFileInfo (program).baseName ()),
    program_(program),
    arguments_(arguments),
    inputs_(inputs),
    sts_(PreparingStatus)
{
    PROCSCHEDULER_TRACE_ENTRY;
    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ProcInvok::~ProcInvok ()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */

/* ========================================================================= */