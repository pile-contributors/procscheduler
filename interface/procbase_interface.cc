/**
 * @file procbase_interface.cc
 * @brief Definitions for IProcBase class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2016 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "procbase_interface.h"
#include "procscheduler-private.h"
#include "procjob_interface.h"
#include "procinvocation_interface.h"
#include <QStringList>

/**
 * @class IProcBase
 *
 * .
 */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcBase::IProcBase (ProcScheduler *mng) :
    mng_(mng)
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * .
 */
IProcBase::~IProcBase()
{
    PROCSCHEDULER_TRACE_ENTRY;

    PROCSCHEDULER_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * The method iterates all kids and compares provided name
 * with each one. If not found NULL is returned.
 */
IProcBase * IProcBase::findKid (
        const QString &s_name) const
{
    foreach (IProcBase * iter, kids()) {
        if (iter->label () == s_name) {
            return iter;
        }
    }
    return NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcInvok * IProcBase::toInvok ()
{
    if (baseType () == InvocationType) {
        return static_cast<IProcInvok*>(this);
    } else {
        return NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
IProcJob *IProcBase::toJob ()
{
    if (baseType () == JobType) {
        return static_cast<IProcJob*>(this);
    } else {
        return NULL;
    }
}
/* ========================================================================= */
