ProcScheduler
=============

Management of jobs consisting of one or more process invocation.

The basic run unit is a ProcInvocation that may be grouped
together for management purposes in ProcJob classes. The invocations
are similar to ProcRun instances.

Each ProcInvocation may depend on the (successful) completion of one
or mode ProcInvocation instances or of entire jobs and,
once these are satisfied, it is added
to the internal queue. The user may customize the number or instances that
can run simultaneously.

The dependency tree is builded by using the signal - slot mechanism.
Each time a ProcInvocation or ProcJob is done it emmits a signal.
The dependent parties then examine the status of all other dependencies
and, if all are completed, adds itself to the queue.
