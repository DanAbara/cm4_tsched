/**
 ******************************************************************************
 * @file           : led.h
 * @author         : D. Abara
 * @brief          : Simple script to toggle internal led on stm32F401RE MCU
 * 					 plus 3 external leds connected to the MCU
 ******************************************************************************
 *
 * This software is licensed under MIT license.
 *
 ******************************************************************************
 */

#include "led.h"

void init_leds_all(void)
{
	/* Configure GPIO Port A pins 5 to 8 */
	*RCC_AHB1ENR |= ( 1 << 0 ); 								// enable GPIOA

	*GPIOA_MODER &= ~( 0x3FC00 ); 								// clear pins 5-8
	*GPIOA_MODER |= ( 1 << (2 * LED_GREEN) );
	*GPIOA_MODER |= ( 1 << (2 * LED_RED_EXT) );
	*GPIOA_MODER |= ( 1 << (2 * LED_GREEN_EXT) );
	*GPIOA_MODER |= ( 1 << (2 * LED_BLUE_EXT) );

	// Configure PA5-PA8 as output push pull
	*GPIOA_OTYPER &= ~( 1 << LED_GREEN );
	*GPIOA_OTYPER &= ~( 1 << LED_RED_EXT );
	*GPIOA_OTYPER &= ~( 1 << LED_GREEN_EXT );
	*GPIOA_OTYPER &= ~( 1 << LED_BLUE_EXT );

	// Configure PA5-PA8 for High-speed
	*GPIOA_OSPEEDR &= ~( 0x3FC00 );								// clear register value
	*GPIOA_OSPEEDR |= ( 0x2A800 );

	// Configure PA5-PA8 as no pull-up and no pull-down
	*GPIOA_PUPDR &= ~( 0x3FC00 );

	// initially turn off all LEDs
	led_off(LED_GREEN);
	led_off(LED_RED_EXT);
	led_off(LED_GREEN_EXT);
	led_off(LED_BLUE_EXT);
}

void led_on(uint8_t led_no)
{
	// turn on led
	*GPIOA_ODR |= ( 1 << led_no );
	// _delay(5000000);
	// led_off(led_no);
}

void led_off(uint8_t led_no)
{
	// turn off led
	*GPIOA_ODR &= ~( 1 << led_no );
	// _delay(500000);
}

void _delay(uint32_t count)
{
	for (uint32_t i = 0; i < count; i++);
}










