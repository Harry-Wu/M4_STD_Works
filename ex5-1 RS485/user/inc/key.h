#ifndef _KEY_H_
#define _KEY_H_
#include "io_bit.h"

//KEY1~KEY4为板子上丝印的名字
#define KEY1 PAin(0)
#define KEY2 PEin(4)
#define KEY3 PEin(3)
#define KEY4 PEin(2)

#define NO_KEY 0xff
#define KEY1_OK 1

void key_init(void);
u8 key_scanf(void);

#endif
