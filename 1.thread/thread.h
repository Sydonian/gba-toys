#ifndef THREAD_H_
#define THREAD_H_

#include "values.h"
#include "timer.h"

#define STACK_INIT_OFFSET -6
#define STACK_LR_OFFSET 5
#define STACK_OFFSET 0x3000

#define CONTEXT_LRINDEX 5

#define THREAD_READY 0x01
#define THREAD_RUNNING 0x02
#define THREAD_EXIT 0x03

#define THREAD_SCHEDULE_TIMER TIMER_ID3

/*
 * 线程的状态
 */
typedef struct _tagThreadContext
{
    /*如果改动了这里的顺序或者位置，就要同步修改int_warp.s*/
    uint32 *SP;
    uint32 R0R4R12LR[6];
    uint32 R4R11[8];
    /*如果改动了这里的顺序或者位置，就要同步修改int_warp.s*/

    void * StackPtr;
    void (*Func)();
    uint32 State;
} ThreadContext;

/*
 * 线程队列
 */
typedef struct _tagContextLinkedList ContextLinkedList;
struct _tagContextLinkedList
{
    ThreadContext *Context;
    ContextLinkedList *Next;
    ContextLinkedList *Last;
};

/*
 * 调度器
 */
typedef struct _tagThreadSchedulerState
{
    ThreadContext *Current;
    ContextLinkedList *Queue;


} ThreadSchedulerState;
extern ThreadSchedulerState ScheduleState;


/*
 * 指定线程函数和栈大小，创建ThreadContext
 */
ThreadContext* GBAThreadCreateContext(void (*func)(),uint32 stackSize);
/*
 * 将指定的ThreadContext添加到线程队列中
 */
void GBAThreadRegister(ThreadContext *context);


/*
 * 一些循环队列操作函数
 */
void __queue_insert_before(ContextLinkedList *chain,ContextLinkedList *node);
ContextLinkedList* __queue_remove_and_last(ContextLinkedList *chain);
int __queue_is_last_one(ContextLinkedList *chain);


/*
 * 调度器初始化的时候，需要设置一个ThreadContext。一般都是用main函数
 */
void __thread__regis_as_main(ThreadContext *context);
/*
 * 回收线程
 */
void __thread__finalize(ContextLinkedList *node);

/* 
 * 请求调度线程
 */
void __call_schedule();

/* 
 * 调用ThreadContext中的线程函数，以及处理线程函数结束后的清理工作（未完成）
 */
void __thread_entry(ThreadContext *context);

/*
 * 用于从线程栈中读取出之前存入的ThreadContext对象
 * 生成汇编代码用的模板函数
 */
void __thread_entry_thunk();

#endif
