#ifndef _LED_H_
#define	_LED_H_
#include "io_bit.h"

#define BUZZER1 PFout(8)
#define LED5 PFout(9)
#define LED6 PFout(10)

typedef enum
{
	LED_OFF = 1,
	LED_ON = 0
}LEDState_Typedef;

void led_init(void);

#endif
