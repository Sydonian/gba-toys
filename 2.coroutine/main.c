#include <stdlib.h>
#include "coroutine.h"

void F1(Coroutine *con, void* args)
{
    int *ret = malloc(sizeof(int));
    *ret = 0;
    while(1)
    {
        (*ret)++;
        CoYield(con,ret);
    }
}

void F2(Coroutine *con, void* args)
{
    int *ret = malloc(sizeof(int));
    *ret = 0;
    while(1)
    {
        (*ret)--;
        CoYield(con,ret);
    }
}

int main()
{
    CoContext *context = CoCreateContext(512);
    CoContext *context2 = CoCreateContext(256);
    Coroutine *co1 = CoCreateCoroutine(context,F1,0);
    Coroutine *co2 = CoCreateCoroutine(context2,F2,0);

    unsigned int timer = 100000;
    while(timer) timer--;

    int r1 = 10;
    int r2 = 10;
    while(1)
    {
        r1 = *(int*)CoResume(co1);
        r2 = *(int*)CoResume(co2);
    }


}
