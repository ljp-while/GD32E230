#include "led.h"

void led_init(void)
{
    /* enable the LED1 GPIO clock */
    rcu_periph_clock_enable(LED_CLOCK);	
	/* configure LED1 GPIO port */ 
    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PIN);
	led_on();
}


void led_on(void)
{
	gpio_bit_reset(LED_PORT, LED_PIN);
}

void led_off(void)
{
	gpio_bit_set(LED_PORT, LED_PIN);
}