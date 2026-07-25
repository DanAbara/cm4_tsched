/**
 ******************************************************************************
 * @file           : main.h
 * @author         : D. Abara
 * @brief          : Cortex M4 bare metal example
 ******************************************************************************
 *
 * This software is licensed under MIT license.
 *
 ******************************************************************************
 */
#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdio.h>

/**
 * Stack memory allocation
 * RAM size: 96KB
 */
#define SIZE_TASK_STACK		( 1024U ) // 1KB
#define SIZE_SCHED_STACK	( 1024U ) // 1KB

#define SRAM_START			( 0x20000000U )
#define SIZE_SRAM			( (96) * (1024) )
#define SRAM_END			( (SRAM_START) + (SIZE_SRAM) )

/* configs and initialization */
void initialise_monitor_handles(void);  // called from C stdlib for initializing semi-hosting


#endif // MAIN_H_
