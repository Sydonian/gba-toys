#ifndef REGDEFS_H_
#define REGDEFS_H_

#include "memmap.h"
#include "values.h"


/*#define BITS(name,len) uint16 name : len;
#define BIT(name) uint16 name : 1;
#define UNUSED(len) uint16 : len;


#define DEF_REG(name,offset,part) \
typedef union _tagDefReg_## name { \
struct  { \
part \
}; uint16 WORD; } DEFREG_##name ; \
DEFREG_##name *REG_##name = (DEFREG_##name*)((offset) + REGISTER_OFFSET);
*/

#endif
