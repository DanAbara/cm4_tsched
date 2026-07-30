#include <stdint.h>
#include "sched.h"

/* Array of task stack addresses */
uint32_t task_stack_addr[MAX_TASKS] = { T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START };
Task user_tasks[MAX_TASKS];


/* Task Handlers Functions */
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

__attribute__ ((naked)) void init_scheduler_stack(uint32_t scheduler_stack_start)
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

	const uint32_t kZeroInitRegs = 13;
	
	/* set PSP values for each task */
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

void init_systick_timer(void)
{
	/**
	 * setup systick timer to interrupt the processor at fixed intervals
	 * F401RE usses internlal freq of 16MHz. 
	 */
	uint32_t *pSYST_RVR = ( (uint32_t*)(0xE000E014) );	// systick reload val reg
	uint32_t *pSYST_CSR = ( (uint32_t*)(0xE000E010) );	// systick control and status reg

	uint32_t load_value = ( CLOCK_FREQ_HZ ) - 1; 		// interrupt every 1 second

	*pSYST_RVR &= ~(0x00FFFFFF);						// clear load value in first 24 bits
	*pSYST_RVR |= load_value;

	*pSYST_CSR &= ~(0x00000007);						// clear bits 0,1,2
	*pSYST_CSR |= (1 << 2);								// use processor clock
	*pSYST_CSR |= (1 << 1);								// TICKINT countdown triggers systick exception

	*pSYST_CSR |= (1 << 0);								// ENABLE systick counter

}

void Systick_Handler(void)
{
	/**
	 * This handler's function is to trigger the PendSV exception each time a new task is to be run
	 */
	printf("In systick handler\n");
	uint32_t *pICSR = ( (uint32_t*)(0xE000ED04) );		// Interrupt control and state reg
	*pICSR |= (1 << 28); 								// Pend the PendSV exception
}

__attribute__ ((naked)) void PendSV_Handler(void)
{
	/**
	 * Perform context switch from one task to another
	 */
	__asm volatile ("BX LR");
}