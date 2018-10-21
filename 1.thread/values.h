/*
 * 定义的一些类型和宏，方便编程
 */

#ifndef VALUES_H_
#define VALUES_H_

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;

#define D0 0x0001
#define D1 0x0002
#define D2 0x0004
#define D3 0x0008

#define D4 0x0010
#define D5 0x0020
#define D6 0x0040
#define D7 0x0080

#define D8 0x0100
#define D9 0x0200
#define D10 0x0400
#define D11 0x0800

#define D12 0x1000
#define D13 0x2000
#define D14 0x4000
#define D15 0x8000


#define Slice0(value,len) (~(-1<<(len) & value)
#define SliceShf(value,len,shf) (Slice0(value,len) << shf)
#define SelectSlice(_1,_2,_3,name,...) name
#define Slice(args...) (SelectSlice(args,SliceShf,Slice0,...)(args))

#define SetFlag(value,flag) ((value) |= (flag))
#define ClearFlag(value,flag) ((value) &= ~(flag))

#endif
