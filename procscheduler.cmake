
# enable/disable cmake debug messages related to this pile
set (PROCSCHEDULER_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (procschedulerInit
          ref_cnt_use_mode)

    # default name
    if (NOT PROCSCHEDULER_INIT_NAME)
        set(PROCSCHEDULER_INIT_NAME "ProcScheduler")
    endif ()

    # compose the list of headers and sources
    set(PROCSCHEDULER_HEADERS
        "procscheduler.h")
    set(PROCSCHEDULER_SOURCES
        "procscheduler.cc")

    pileSetSources(
        "${PROCSCHEDULER_INIT_NAME}"
        "${PROCSCHEDULER_HEADERS}"
        "${PROCSCHEDULER_SOURCES}")

    pileSetCommon(
        "${PROCSCHEDULER_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
