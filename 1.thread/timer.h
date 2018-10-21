/*
 * 控制GBA的定时器
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "values.h"
#include "memmap.h"


#define TIMER_DATA_OFFSET 0x100
#define TIMER_SETTING_OFFSET 0x102

/* 总共4个定时器 */
#define TIMER_ID0 0
#define TIMER_ID1 1
#define TIMER_ID2 2
#define TIMER_ID3 3

/* 计时间隔(Counter + 1经过的机器周期) */
#define TIMER_SCALE_0 0
#define TIMER_SCALE_64 1
#define TIMER_SCALE_256 2
#define TIMER_SCALE_1024 3

#define TIMER_ENABLE_SCALE D2
#define TIMER_ENABLE_INT D6
#define TIMER_ENABLE D7

/* 计时器寄存器的地址 */
#define TIMER_DATA_PTR(id) (uint16*)(TIMER_ID##id * 4 + TIMER_DATA_OFFSET + REGISTER_OFFSET)
#define TIMER_SETTING_PTR(id) (uint16*)(TIMER_ID##id * 4 + TIMER_SETTING_OFFSET + REGISTER_OFFSET)

/* 准确来说是设置计时器重新开始的数值 */
#define GBATimerGetData(id) (*TIMER_DATA_PTR(id))
#define GBATimerSetData(id,value) (*TIMER_DATA_PTR(id) = (value))

#define GBATimerGetSetting(id) (*TIMER_SETTING_PTR(id))

#define GBATimerSetScale(id,scale) (GBATimerGetSetting(id) = (scale))

/* 是否忽略计时间隔的设定 */
#define GBATimerEnableScale(id) (ClearFlag(GBATimerGetSetting(id),TIMER_ENABLE_SCALE))
#define GBATimerDisableScale(id) (SetFlag(GBATimerGetSetting(id),TIMER_ENABLE_SCALE))

/* 启用计时器溢出中断 */
#define GBATimerEnableInt(id) (SetFlag(GBATimerGetSetting(id),TIMER_ENABLE_INT))
#define GBATimerDisableInt(id) (ClearFlag(GBATimerGetSetting(id),TIMER_ENABLE_INT))

/* 启动计时器中断 */
#define GBATimerEnable(id) (SetFlag(GBATimerGetSetting(id),TIMER_ENABLE))
#define GBATimerDisable(id) (ClearFlag(GBATimerGetSetting(id),TIMER_ENABLE))


#endif
