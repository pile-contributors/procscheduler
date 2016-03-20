if (NOT PROCSCHEDULER_BUILD_MODE)
    set (PROCSCHEDULER_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (ProcScheduler)
procschedulerInit(${PROCSCHEDULER_BUILD_MODE})
