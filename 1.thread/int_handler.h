#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_

/* 
 * 用于在触发中断的瞬间保存好之前线程的状态，
 * 然后在调用了中断处理程序(GBAInterruptHandler)之后，
 * 切换到调度器中指定线程(Current)的状态
 * 
 * 生成汇编代码用的模板函数
 */
void int_handler_warp();

/*
 * 中断处理程序，在这里选择下一次要执行的线程(调度)
 */
void GBAInterruptHandler();

#endif
