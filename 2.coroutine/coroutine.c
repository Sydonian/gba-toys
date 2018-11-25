#include <stdlib.h>
#include <string.h>
#include "coroutine.h"

Coroutine *__current;
void *__args;
jmp_buf __init;

CoContext* CoCreateContext(size_t stackSize)
{
	CoContext *con = (CoContext*)malloc(sizeof(CoContext));
	con->stack = malloc(stackSize);
	con->stackSize = stackSize;
	return con;
}

void CoDestroyContext(CoContext *con)
{
	free(con->stack);
	free(con);
}

Coroutine* CoCreateCoroutine(CoContext *context, CoroutineProc proc, void *args)
{
	Coroutine *con = (Coroutine*)malloc(sizeof(Coroutine));
	con->context = context;
	con->proc = proc;
	con->stackDumps = 0;
	con->dumpsLen = 0;
	con->finished = 0;
	con->ret = 0;



	if (setjmp(con->regs))
	{
		__co_entry();
	}
	else
	{
		//准备好要传递的参数
		__current = con;
		__args = args;
		if (!setjmp(__init))
		{
			//将jmp_buf中的栈指针设置为共享栈的栈顶
			CoSetSP(con->regs, CoContextStackTopAddr(context) - 16); //实践发现重新（longjmp）到setjmp的位置时，会增加栈指针，所以减少一点
			longjmp(con->regs, 1); //设置好了jmp_buf中的栈指针，就可以切换了
		}
	}

	return con;
}
void CoDestroyCoroutine(Coroutine *con)
{
	if (con->stackDumps)
		free(con->stackDumps);
	free(con);
}

void* CoResume(Coroutine *con)
{
	//保存caller的状态
	if (!setjmp(con->context->resumeRegs))
	{
		//恢复协程的堆栈内容
		void* addr = CoStackContentAddr(con);
		memcpy(addr, con->stackDumps, con->dumpsLen);
		//切换到协程
		longjmp(con->regs, 1);
	}

	return con->ret;
}
void CoYield(Coroutine *con, void *ret)
{
	__save_stack(con);
	if (!setjmp(con->regs))
	{
		//设置返回值
		con->ret = ret;
		//切回caller
		longjmp(con->context->resumeRegs, 1);
	}
}


void __co_entry()
{
	//将参数保存到栈里，因为__current是共用的
	Coroutine *con = __current;
	void *args = __args;
	__save_stack(con);
	if (setjmp(con->regs))
	{
		con->proc(con, args);
		con->finished = 1;
		while (1) //协程函数运行完，但不能退出这个函数
			CoYield(con, con->ret);
	}
	longjmp(__init, 1);
}

void __save_stack(Coroutine *con)
{
	void *topAddr = CoGetStackTopAddr();
	int len = CoContextStackTopAddr(con->context) - (int)topAddr;
	if (len > con->dumpsLen)
	{
		if (con->stackDumps)
			free(con->stackDumps);
		void *ptr = malloc(len);
		con->stackDumps = ptr;
	}
	memcpy(con->stackDumps, topAddr, len);
	con->dumpsLen = len;
}







