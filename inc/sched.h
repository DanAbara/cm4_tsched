/**
 ******************************************************************************
 * @file           : sched.h
 * @author         : D. Abara
 * @brief          : Cortex M4 bare metal simple scheduler header
 ******************************************************************************
 *
 * This software is licensed under MIT license.
 *
 ******************************************************************************
 */
#ifndef SCHED_H_
#define SCHED_H_

#include <stdint.h>
#include <stdio.h>

/**
 * Stack memory allocation
 * RAM size: 96KB
 */
#define SIZE_TASK_STACK		( 1024U ) // 1KB

#define SRAM_START			( 0x20000000U )
#define SRAM_SIZE			( (96) * (1024) )
#define SRAM_END			( (SRAM_START) + (SRAM_SIZE) )

#define T0_STACK_START      ( SRAM_END )
#define T1_STACK_START      ( (SRAM_END) - (SIZE_TASK_STACK) )
#define T2_STACK_START      ( (SRAM_END) - (SIZE_TASK_STACK * 2) )
#define T3_STACK_START      ( (SRAM_END) - (SIZE_TASK_STACK * 3) )
#define SCHED_STACK_START   ( (SRAM_END) - (SIZE_TASK_STACK * 4) )

#define MAX_TASKS           ( 4 )

#define CLOCK_FREQ_HZ       ( 16000000U )


/* configs and initialization */
extern void initialise_monitor_handles(void);  // called from C stdlib for initializing semi-hosting
void enable_processor_faults(void);
void init_scheduler_stack(uint32_t scheduler_stack_start);
void init_task_stacks(void);
void init_systick_timer(void);
void switch_sp_to_psp(void);
void task0_handler(void);
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void save_psp_value(uint32_t curr_psp_value);
void decide_next_task(void);
uint32_t get_psp_value(void);

typedef struct
{
    uint32_t psp_value;                 // psp value of task
    void (*task_handler) (void);        // address of taskx_handler for x = 1,...,MAX_TASKS
} Task;


#endif // SCHED_H_
