#include <stdint.h>
#include "sched.h"
#include "led.h"

/* Array of task stack addresses */
uint32_t task_stack_addr[MAX_TASKS] = { T0_STACK_START, T1_STACK_START, T2_STACK_START, T3_STACK_START };
Task user_tasks[MAX_TASKS];
uint8_t current_task = 0; // start with task 0

const uint32_t count_1ms = 1250;
const uint32_t count_1s = 1000 * count_1ms;
const uint32_t count_500ms = 500 * count_1ms;
const uint32_t count_250ms = 250 * count_1ms;
const uint32_t count_125ms = 125 * count_1ms; 


/* Task Handlers Functions */
void task0_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		_delay(count_1s);
		led_off(LED_GREEN);
		_delay(count_1s);
	}
}

void task1_handler(void)
{
	while(1)
	{
		led_on(LED_RED_EXT);
		_delay(count_500ms);
		led_off(LED_RED_EXT);
		_delay(count_500ms);
	}
}

void task2_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN_EXT);
		_delay(count_250ms);
		led_off(LED_GREEN_EXT);
		_delay(count_250ms);
	}
}

void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE_EXT);
		_delay(count_125ms);
		led_off(LED_BLUE_EXT);
		_delay(count_125ms);
	}
}

/* Trap faults */ 
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
	user_tasks[0].task_handler = task0_handler;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;

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

	uint32_t load_value = ( CLOCK_FREQ_HZ / 1000U ) - 1; // interrupt every 1ms (1kHz)

	*pSYST_RVR &= ~(0x00FFFFFF);						// clear load value in first 24 bits
	*pSYST_RVR |= load_value;

	*pSYST_CSR &= ~(0x00000007);						// clear bits 0,1,2
	*pSYST_CSR |= (1 << 2);								// use processor clock
	*pSYST_CSR |= (1 << 1);								// TICKINT countdown triggers systick exception

	*pSYST_CSR |= (1 << 0);								// ENABLE systick counter

}

// void Systick_Handler(void)
// {
// 	/**
// 	 * This handler's function is to trigger the PendSV exception each time a new task is to be run
// 	 */
// 	printf("In systick handler\n");
// 	uint32_t *pICSR = ( (uint32_t*)(0xE000ED04) );		// Interrupt control and state reg
// 	*pICSR |= (1 << 28); 								// Pend the PendSV exception
// }

__attribute__ ((naked)) void Systick_Handler(void)
{
	/**
	 * Perform context switch from one task to another
	 */

	 /* Save context of current task */
	// get psp value of current task
	__asm volatile ("MRS R0, PSP");

	// save SF2 (R4-R11) to task's private stack
	__asm volatile ("STMDB R0!, {R4-R11}");

	__asm volatile ("PUSH {LR}"); // save EXC_RETURN value to R3 for exiting later

	// save psp value
	__asm volatile ("BL save_psp_value");

	/* Retrieve context of next task */
	// get next task
	__asm volatile ("BL decide_next_task");

	// get psp value of next task
	__asm volatile ("BL get_task_psp_value");

	// load SF2 from task's private stack
	__asm volatile ("LDMIA R0!, {R4-R11}");

	// update PSP reg to run next task
	__asm volatile ("MSR PSP, R0");

	__asm volatile ("POP {LR}"); // retrieve EXC_RETURN back from R3 and use to exit
	__asm volatile ("BX LR");
}

__attribute__ ((naked)) void switch_sp_to_psp(void)
{
	/**
	 * Switch the stack pointer to PSP to execute user tasks
	 * Since the stacks have been initialized for all tasks, its psp value will be such that both initial SF1 & SF2 are
	 * in their respective stacks and can be retrieved to run the task
	 */
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL get_task_psp_value");
	__asm volatile ("MSR PSP, R0");			// copy current psp of task0 stack
	__asm volatile ("POP {LR}");

	__asm volatile ("MOV R0, #0x02"); 		// store 0b0010 in R0
	__asm volatile ("MSR CONTROL, R0");		// switch from MSP to PSP
	__asm volatile ("BX LR"); 				// return to main
}

void save_psp_value(uint32_t curr_psp_value)
{
	/**
	 * Save the psp value of the current task so that it can be retrieved later
	 */
	user_tasks[current_task].psp_value = curr_psp_value; // curr_psp_value passed from R0 as per AAPCS
}

void decide_next_task(void)
{
	/** 
	 * Decide the next task to be run
	 * Has to be in the range {0,1,..3}
	 */
	current_task++;
	current_task %= MAX_TASKS;
}

uint32_t get_task_psp_value(void)
{
	/**
	 * Get the psp value of a given task, which is the value of PSP
	 */
	uint32_t psp_val = user_tasks[current_task].psp_value;
	return psp_val; // psp_val stored in R0 per AAPCS
}