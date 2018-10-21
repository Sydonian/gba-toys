#include "interrupt.h"
#include "thread.h"

void GBAInterruptHandler()
{
    GBAIntResetAll();
    GBATimerSetData(THREAD_SCHEDULE_TIMER ,0);

    //if(ScheduleState.Current->State == THREAD_RUNNING)
    {
        ScheduleState.Current->State = THREAD_READY;
    }

    ContextLinkedList *con = ScheduleState.Queue->Last;

    while(!__queue_is_last_one(con) && con->Context->State != THREAD_READY)
    {
        if(con->Context->State == THREAD_EXIT)
        {
            ContextLinkedList *tmp = con;
            con = __queue_remove_and_last(con);
            __thread__finalize(tmp);
        }
        else
        {
            con = con->Last;
        }
    }
    
    con->Context->State = THREAD_RUNNING;
    ScheduleState.Queue = con;
    ScheduleState.Current = con->Context;

}
