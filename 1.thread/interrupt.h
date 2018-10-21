/*
 * GBA的中断处理
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "memmap.h"
#include "values.h"
/*
DEF_REG(IE,0x200,
BIT(VBLANK)
BIT(HBLANK)
BIT(VCOUNTER)
BIT(TIMER0)
BIT(TIMER1)
BIT(TIMER2)
BIT(TIMER3)
BIT(ALL_SIO)
BIT(DMA0)
BIT(DMA1)
BIT(DMA2)
BIT(DMA3)
BIT(KEY)
BIT(CARTRIDGE)
UNUSED(2))


DEF_REG(IF,0x202,
BIT(VBLANK)
BIT(HBLANK)
BIT(VCOUNTER)
BIT(TIMER0)
BIT(TIMER1)
BIT(TIMER2)
BIT(TIMER3)
BIT(ALL_SIO)
BIT(DMA0)
BIT(DMA1)
BIT(DMA2)
BIT(DMA3)
BIT(KEY)
BIT(CARTRIDGE)
UNUSED(2))

DEF_REG(IME,0x208,
BIT(FLAG)
UNUSED(15)
)

*/
#define INT_VBLANK D0
#define INT_HBLANK D1
#define INT_VCOUNTER D2
#define INT_TIMER0 D3
#define INT_TIMER1 D4
#define INT_TIMER2 D5
#define INT_TIMER3 D6
#define INT_ALL_SIO D7
#define INT_DMA0 D8
#define INT_DMA1 D9
#define INT_DMA2 D10
#define INT_DMA3 D11
#define INT_KEY D12
#define INT_CARTRIDGE D13

#define INT_IME_FLAG D0

#define IE_PTR (uint16*)(0x200 + REGISTER_OFFSET)
#define IF_PTR (uint16*)(0x202 + REGISTER_OFFSET)
#define IME_PTR (uint16*)(0x208 + REGISTER_OFFSET)

/* 中断处理程序的地址 */
#define INT_HANDLER_PTR (uint32*)(0x7FFC + INTERNAL_RAM_OFFSET)

#define GBAIntEnableAll() do{SetFlag(*IME_PTR,INT_IME_FLAG);}while(0)
#define GBAIntDisableAll() do{ClearFlag(*IME_PTR,INT_IME_FLAG);}while(0)

#define GBAIntEnable(flag) do{SetFlag(*IE_PTR,flag);}while(0)
#define GBAIntDisable(flag) do{ClearFlag(*IE_PTR,flag);}while(0)

#define GBAIntIsInt(flag) (*IF_PTR & (flag))
#define GBAIntResetInt(flag) (SetFlag(*IF_PTR,(flag)))
#define GBAIntResetAll() do{*IF_PTR = -1;}while(0)

#define GBAIntSetIntHandler(func) do{*INT_HANDLER_PTR = (uint32)func;}while(0)

#endif
