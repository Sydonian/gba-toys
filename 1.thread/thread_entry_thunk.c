#include "thread.h"

void __thread_entry_thunk()
{
    ThreadContext *ptr;
    asm("ldmfd sp!,{%0}":"=r"(ptr));
    __thread_entry(ptr);
}