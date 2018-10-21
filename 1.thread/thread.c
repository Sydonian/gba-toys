#include "thread.h"
#include "memmap.h"
#include "timer.h"
#include "malloc.h"

ThreadSchedulerState ScheduleState = {0,0};

ThreadContext* GBAThreadCreateContext(void (*func)(),uint32 stackSize)
{
    ThreadContext *ptr = (ThreadContext*)malloc(sizeof(ThreadContext));

    //给main函数准备的线程，不需要设置任何内容
    if(stackSize > 0)
    {
        ptr->StackPtr = malloc(stackSize);
        

        ptr->SP = (uint32*)((uint32)ptr->StackPtr + stackSize);

        ptr->SP--;
        *(ptr->SP) = (uint32)ptr;

        ptr->R0R4R12LR[CONTEXT_LRINDEX] = (uint32)__thread_entry_thunk + 4;
        ptr->Func = func;
    }
    return ptr;
}
void GBAThreadRegister(ThreadContext *context)
{
    ContextLinkedList *node = (ContextLinkedList*)malloc(sizeof(ContextLinkedList));
    node->Context = context;
    node->Context->State = THREAD_READY;
    __queue_insert_before(ScheduleState.Queue,node);
}
void __queue_insert_before(ContextLinkedList *chain,ContextLinkedList *node)
{
    node->Next = chain->Next;
    chain->Next->Last = node;
    node->Last = chain;
    chain->Next = node;
}
ContextLinkedList* __queue_remove_and_last(ContextLinkedList *chain)
{
    chain->Last->Next = chain->Next;
    chain->Next->Last = chain->Last;
}
int __queue_is_last_one(ContextLinkedList *chain)
{
    return chain->Next == chain;
}

void __call_schedule()
{
    GBATimerSetData(THREAD_SCHEDULE_TIMER,-1);
    GBATimerDisable(THREAD_SCHEDULE_TIMER);
    GBATimerEnable(THREAD_SCHEDULE_TIMER);
    GBATimerSetData(THREAD_SCHEDULE_TIMER,0);
}

void __thread__regis_as_main(ThreadContext *context)
{
    ContextLinkedList *node = (ContextLinkedList*)malloc(sizeof(ContextLinkedList));
    node->Context = context;
    ScheduleState.Queue = node;
    ScheduleState.Current = context;
    node->Last = node;
    node->Next = node;
    node->Context->State = THREAD_READY;
}

void __thread__finalize(ContextLinkedList *node)
{
    free(node->Context->StackPtr);
    free(node->Context);
    free(node);
}

void __thread_entry(ThreadContext *context)
{
    context->Func();
    context->State = THREAD_EXIT;

    while(1) 
        __call_schedule();
}


