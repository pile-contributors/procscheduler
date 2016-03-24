
# enable/disable cmake debug messages related to this pile
set (PROCSCHEDULER_DEBUG_MSG OFF)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (procschedulerInit
          use_mode)

    # default name
    if (NOT PROCSCHEDULER_INIT_NAME)
        set(PROCSCHEDULER_INIT_NAME "ProcScheduler")
    endif ()

    # compose the list of headers and sources
    set(PROCSCHEDULER_HEADERS
        "interface/procjob_interface.h"
        "interface/procbase_interface.h"
        "interface/procinvocation_interface.h"
        "interface/procinvocation_proc.h"
        "inmem/procfactory_inmem.h"
        "inmem/procinvocation_inmem.h"
        "inmem/procjob_inmem.h"
        "procscheduler.h"
        )

    set(PROCSCHEDULER_SOURCES
        "interface/procbase_interface.cc"
        "interface/procinvocation_interface.cc"
        "interface/procinvocation_proc.cc"
        "interface/procjob_interface.cc"
        "inmem/procinvocation_inmem.cc"
        "inmem/procjob_inmem.cc"
        "inmem/procfactory_inmem.cc"
        "procscheduler.cc"
        )





    set(PROCSCHEDULER_QT_MODS
        "Core"
        )

    pileSetSources(
        "${PROCSCHEDULER_INIT_NAME}"
        "${PROCSCHEDULER_HEADERS}"
        "${PROCSCHEDULER_SOURCES}")

    pileSetCommon(
        "${PROCSCHEDULER_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${use_mode}"
        "ProcRun"
        "runtime"
        "nogui;process")

endmacro ()
