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

#ifndef LED_H_
#define LED_H_

#include <stdio.h>
#include <stdint.h>

// stm32F401RE-NUCLEO, setting up GPIOA, PA5 (internal LED), PA6, PA7, PA8
#define GPIOA_BASE			( 0x40020000 )									// GPIO port A base addr
#define RCC_BASE			( 0x40023800 )  								// RCC base address

/* GPIO Port Registers */
#define RCC_AHB1ENR  		( (volatile uint32_t*)(RCC_BASE + 0x30) ) 		// RCC AHB1 peripheral clock enable reg., AHB1ENR offset = 0x30,
#define GPIOA_MODER 		( (volatile uint32_t*)(GPIOA_BASE) )			// GPIO mode register
#define GPIOA_OTYPER  		( (volatile uint32_t*)(GPIOA_BASE + 0x04) ) 	// GPIO output type register
#define GPIOA_OSPEEDR  		( (volatile uint32_t*)(GPIOA_BASE + 0x08) ) 	// GPIO output speed register
#define GPIOA_PUPDR 		( (volatile uint32_t*)(GPIOA_BASE + 0x0C) )		// GPIO Pull-up Pull-down register
#define GPIOA_ODR  			( (volatile uint32_t*)(GPIOA_BASE + 0x14) )		// GPIO output data register

/* User LEDs - 1 onboard, 3 external */
#define LED_GREEN			( (uint8_t)(5) ) 								// PA5, internal user LED2
#define LED_RED_EXT			( (uint8_t)(6) ) 								// PA6, external LED
#define LED_GREEN_EXT		( (uint8_t)(7) ) 								// PA7, external LED
#define LED_BLUE_EXT		( (uint8_t)(8) ) 								// PA8, external LED

/* prototypes */
void init_leds_all(void);
void led_on(uint8_t led_no);
void led_off(uint8_t led_no);
void _delay(uint32_t count);

#endif
