#include "timer.h"
#include "interrupt.h"
#include "values.h"
#include "thread.h"
#include "int_handler.h"

int flags[4];



uint8 buf1[] = { 5,9,4,6,3,2,7,0,1,8 };
uint8 buf2[10];
void msort(int start, int end, uint8 *src, uint8 *des,int dir)
{
	if (start == end)
	{
		return;
	}
	int mid = (start + end) / 2;
	msort(start, mid, src, des,dir);
	msort(mid + 1, end, src, des,dir);
	int p1 = start, p2 = mid + 1;
	int dpos = 0;

	while (p1 <= mid && p2 <= end)
	{
		if ((src[p1] < src[p2]) ^ 1)
		{
			des[dpos] = src[p1];
			p1++;
		}
		else
		{
			des[dpos] = src[p2];
			p2++;
		}
		dpos++;
	}
	while (p1 <= mid)
	{
		des[dpos++] = src[p1];
		p1++;
	}
	while (p2 <= end)
	{
		des[dpos++] = src[p2];
		p2++;
	}
	dpos--;
	while (dpos >= 0)
	{
		src[start + dpos] = des[dpos];
		dpos--;
	}
}
void f1()
{
    while(1)
    {
        msort(0,9,buf1,buf2,0);
        __call_schedule();
    }
}
void f2()
{
    while(1)
    {
        msort(0,9,buf1,buf2,1);
        __call_schedule();
    }
}


int main()
{
    
    GBATimerSetScale(TIMER_ID3,TIMER_SCALE_0);
    GBATimerEnableScale(TIMER_ID3);
    GBATimerEnableInt(TIMER_ID3);
    GBATimerEnable(TIMER_ID3);


    ThreadContext *main = GBAThreadCreateContext(0,0);
    __thread__regis_as_main(main);//main函数所在的线程


    ThreadContext *c1 = GBAThreadCreateContext(f1,256);
    ThreadContext *c2 = GBAThreadCreateContext(f2,256);
    GBAThreadRegister(c1);
    GBAThreadRegister(c2);

    GBAIntSetIntHandler(int_handler_warp);
    GBAIntEnable(INT_TIMER3);
    GBAIntEnableAll();

    while(1){flags[2]++;}
    return 0;
}
