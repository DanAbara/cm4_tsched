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

#include "led.h"
#include "sched.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


int main(void)
{
	// initialize semihosting to use printf
	initialise_monitor_handles();
	
	// enable faults
	enable_processor_faults();

	// initialize scheduler stacks
	init_scheduler_stack(SCHED_STACK_START);
	
	// initialize the stacks for each task
	init_task_stacks();

	// initialize leds
	printf("Initializing LEDs\n");
	init_leds_all();

	// initialize systick timer
	init_systick_timer();

	// switch to PSP and launch first task
	switch_sp_to_psp();

	task0_handler();

	for(;;);
}




