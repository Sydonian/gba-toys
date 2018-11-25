#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <stddef.h>
#include <setjmp.h>



/*
 * 多个协程共享的栈
 */
typedef struct _tagCoContext
{
	void *stack;
	size_t stackSize;
	jmp_buf resumeRegs;

} CoContext;

struct _tagCoroutine;
typedef void (*CoroutineProc)(struct _tagCoroutine *con,void*);
/*
 * 协程数据结构
 */
typedef struct _tagCoroutine
{
	CoContext *context; /* 指向的共享栈 */
	CoroutineProc proc; /* 协程函数 */
	jmp_buf regs; /* 程序上下文 */
	void *stackDumps; /* Yield时共享栈中的内容 */
	size_t dumpsLen; /* 内容的长度 */
	void *ret; /* Yield的设定的返回值 */
	int finished; /* 协程运行结束 */
} Coroutine;



/*
 * 创建指定大小的共享栈
 */
CoContext* CoCreateContext(size_t stackSize);
void CoDestroyContext(CoContext *con);
/*
 * 创建协程，并传递参数
 */
Coroutine* CoCreateCoroutine(CoContext *context,CoroutineProc proc,void *args);
void CoDestroyCoroutine(Coroutine *con);

/*
 * 恢复执行一个协程
 */
void* CoResume(Coroutine *rou);
/*
 * 协程函数内部中断并返回一个值
 */
void CoYield(Coroutine *rou,void *ret);

/* 协程初始化的时候传递参数用的 */
extern Coroutine *__current; 
extern void *__args;
extern jmp_buf __init;

/* 协程外层函数 */
void __co_entry();
/* 保存共享栈中的数据到当前协程 */
void __save_stack(Coroutine *con);


/* 下面这部分是平台相关的内容，在support里实现 */

/* 计算共享栈的栈顶地址 */
int CoContextStackTopAddr(CoContext *context);
/* 计算协程保存的内容放入栈后的栈顶地址 */
void* CoStackContentAddr(Coroutine *corou);
/* 获取当前的栈顶地址 */
void* CoGetStackTopAddr();
/* 设置jmp_buf中保存有sp的那一项 */
void CoSetSP(jmp_buf buf,int addr);


#endif
