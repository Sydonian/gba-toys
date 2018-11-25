#include "coroutine.h"


#if defined(_M_IX86) //VS2017
int CoContextStackTopAddr(CoContext *context)
{
	return (int)context->stack + (context)->stackSize;
}
void* CoStackContentAddr(Coroutine *corou)
{
	int top = CoContextStackTopAddr(corou->context);
	return (void*)(top - corou->dumpsLen);
}
void *CoGetStackTopAddr()
{
	int a = 10;
	return &a;
}
void CoSetSP(jmp_buf buf,int addr)
{
	buf[4] = addr;
}

#elif defined(__arm__) //GBA
int CoContextStackTopAddr(CoContext *context)
{
	return (int)context->stack + (context)->stackSize;
}
void* CoStackContentAddr(Coroutine *corou)
{
	int top = CoContextStackTopAddr(corou->context);
	return (void*)(top - corou->dumpsLen);
}
void *CoGetStackTopAddr()
{
	int a = 10;
	return &a;
}
void CoSetSP(jmp_buf buf, int addr)
{
	buf[9] = addr;
}
#else
#error "Not Supported yet!"
#endif

