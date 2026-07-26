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

Task user_tasks[MAX_TASKS];

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

	// switch to PSP and launch first task

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


void task1_handler(void)
{
	printf("Task 4 running\n");
}

void task2_handler(void)
{
	printf("Task 4 running\n");
}

void task3_handler(void)
{
	printf("Task 4 running\n");
}

void task4_handler(void)
{
	printf("Task 4 running\n");
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

void init_scheduler_stack(uint32_t scheduler_stack_start)
{
	/**
	 * Initialize MSP for scheduler with correct stack address
	 */
	__asm volatile ("MSR MSP, %0": :"r"(scheduler_stack_start) :);
	__asm volatile ("BX LR");
}

void init_task_stacks(void)
{
	/**
	 * Initialize StackFrames 1 and 2 for all private task stacks with SF1:XPSR,PC,LR,R12,R3,R2,R1,R0
	 * and SF2:R11,R10,R9,R8,R7,R6,R5,R4, with key registers like XPSR set for thumb state with Tbit of 1, 
	 * PC set to address of task handler, and LR used to select exception return to PSP in this case. 
	 * The remaining 13 registers are zero initialized.
	 */

	/* set PSP values for each task */
	const uint32_t kZeroInitRegs = 13;
	
	for (uint32_t i = 0; i < MAX_TASKS; i++)
	{
		user_tasks[i].psp_value = task_stack_addr[i];
	}

	/* set handler addresses */
	user_tasks[0].task_handler = task1_handler;
	user_tasks[1].task_handler = task2_handler;
	user_tasks[2].task_handler = task3_handler;
	user_tasks[3].task_handler = task4_handler;

	uint32_t *p_curr_psp;

	for (uint32_t i = 0; i < MAX_TASKS; i++)
	{
		p_curr_psp = (uint32_t*)user_tasks[i].psp_value;

		p_curr_psp--; // decrement before storing for full descending stack model
		*p_curr_psp = 0x01000000U; // XPSR with Tbit of 1

		p_curr_psp--; 
		*p_curr_psp = (uint32_t)user_tasks[i].task_handler; // address of respective task_handler func

		p_curr_psp--;
		*p_curr_psp = 0xFFFFFFFD; // LR - EXC_RETURN to PSP

		for (uint32_t k = 0; k < kZeroInitRegs; k++)
		{
			// set remaining 13 regs to 0
			p_curr_psp--;
			*p_curr_psp = 0;
		}
		user_tasks[i].psp_value = (uint32_t)p_curr_psp; // save respective psp value after init
	}
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

