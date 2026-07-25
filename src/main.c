/**
 ******************************************************************************
 * @file           : main.c
 * @author         : D. Abara
 * @brief          : Cortex M4 bare metal example
 ******************************************************************************
 *
 * This software is licensed under MIT license.
 *
 ******************************************************************************
 */

#include "main.h"
#include "led.h"
extern void enable_processor_faults(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


int main(void)
{
	// initialize semihosting
	initialise_monitor_handles();
	
	// enable faults
	enable_processor_faults();

	// initialize leds
	printf("Initializing LEDs\n");
	init_leds_all();

	while(1)
	{
		// toggle each led one after the other
		led_on(LED_GREEN);
		led_on(LED_RED_EXT);
		led_on(LED_GREEN_EXT);
		led_on(LED_BLUE_EXT);
	}
	//for(;;);
}



void enable_processor_faults(void)
{
    /**
     * 1. Enable all configurable fault exceptions via SHCSR - base address 0xE000ED24
     * Enable bits 18, 17, 16 for usage fault, bus fault and mem fault respectively
     */
	uint32_t volatile *pSHCSR = (uint32_t*)(0xE000ED24);
	*pSHCSR |= (1 << 16); // mem manage fault
	*pSHCSR |= (1 << 17); // bus fault
	*pSHCSR |= (1 << 18); // usage fault
}

// Implement fault handlers
void HardFault_Handler(void)
{
	printf("Exception: Hardfault\n");
	while(1);
}

void MemManage_Handler(void)
{
	printf("Exception: MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception: Busfault\n");
	while(1);
}

void UsageFault_Handler(void)
{
	printf("Exception: UsageFaul\n");
}

