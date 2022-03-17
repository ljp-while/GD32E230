#ifndef  __LED_H
#define  __LED_H

#include "main.h"

#define LED_CLOCK RCU_GPIOC
#define LED_PORT  GPIOC
#define LED_PIN   GPIO_PIN_13


void led_init(void);
void led_on(void);
void led_off(void);
#endif /*__LED_H*/