#include "thread.h"
#include "int_handler.h"

void int_handler_warp()
{
    //保存线程状态
    asm("# Save context start");
    asm("mov r0,%0"::"r"(&ScheduleState.Current->SP));
    asm("stmia r0!,{sp}^");
    asm("mov r0,%0"::"r"(ScheduleState.Current->R4R11));
    asm("stmia r0!,{r4-r11}");
    asm("ldmfd sp!,{r4-r9}");
    asm("mov r0,%0"::"r"(ScheduleState.Current->R0R4R12LR));
    asm("stmia r0!,{r4-r9}");
    asm("# Save context end");

    GBAInterruptHandler();

    //加载线程状态
    asm("mov r0,%0"::"r"(ScheduleState.Current->R0R4R12LR));
    asm("ldmia r0!,{r4-r9}");
    asm("stmfd sp!,{r4-r9}");

    asm("mov r0,%0"::"r"(ScheduleState.Current->R4R11));
    asm("ldmia r0!,{r4-r11}");

    asm("mov r0,%0"::"r"(&ScheduleState.Current->SP));
    asm("ldmia r0!,{sp}^");
    asm("# Switch end");
}
