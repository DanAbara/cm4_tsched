/**
 ******************************************************************************
 * @file           : stm32_startup.c
 * @author         : D. Abara
 * @brief          : Startup file for STM32F401RE MCU
 * 					 
 ******************************************************************************
 *
 * This software is licensed under MIT license.
 *
 ******************************************************************************
 */
#include<stdint.h>

/* Memory */
#define SRAM_START						( 0x20000000U )
#define SRAM_SIZE						( 96 * 1024 ) 						// 96KB
#define SRAM_END						( (SRAM_START) + (SRAM_SIZE) )
#define STACK_START						( SRAM_END )

// access symbols from linker script using extern - end of text section
extern uint32_t _la_data; 	// end of text section/start of data section in ROM
extern uint32_t _sdata;		// start of data section in RAM
extern uint32_t _edata;		// end of data section in RAM
extern uint32_t _sbss;		// start of bss section in RAM
extern uint32_t _ebss;		// end of bss section in RAM

/* prototype of main */
int main(void);

/* libc prototype */
void __libc_init_array(void);

/* Prototypes */
void Reset_Handler						(void);
__attribute__ ((weak, alias ("Default_Handler"))) void NMI_Handler						(void);
__attribute__ ((weak, alias ("Default_Handler"))) void HardFault_Handler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void MemManage_Handler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void BusFault_Handler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void UsageFault_Handler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SVCall_Handler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DebugMon_Handler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void PendSV_Handler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void Systick_Handler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void WWDG_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI16_PVD_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI21_TAMP_STAMP_IRQHandler		(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI22_RTC_WKUP_IRQHandler		(void);
__attribute__ ((weak, alias ("Default_Handler"))) void FLASH_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void RCC_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI0_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI1_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI2_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI3_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI4_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream0_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream1_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream2_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream3_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream4_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream5_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream6_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void ADC_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI9_5_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_BRK_TIM9_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_UP_TIM10_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_TRG_COM_TIM11_IRQHandler	(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_CC_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM2_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM3_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM4_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C1_EV_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C1_ER_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C2_EV_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C2_ER_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI1_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI2_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART1_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART2_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI15_10_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI17_RTC_Alarm_IRQHandler		(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI18_OTG_FS_WKUP_IRQHandler	(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream7_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SDIO_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM5_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI3_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream0_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream1_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream2_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream3_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream4_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_FS_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream5_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream6_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream7_IRQHandler			(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART6_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C3_EV_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C3_ER_IRQHandler				(void);
__attribute__ ((weak, alias ("Default_Handler"))) void FPU_IRQHandler					(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI4_IRQHandler					(void);
	
/* Vector table */
__attribute__ ((section (".isr_vector"))) uint32_t vectors[] = {
	STACK_START,
	(uint32_t)&Reset_Handler,
	(uint32_t)&NMI_Handler,
	(uint32_t)&HardFault_Handler,
	(uint32_t)&MemManage_Handler,
	(uint32_t)&BusFault_Handler,
	(uint32_t)&UsageFault_Handler,
	0,
	0,
	0,
	0,
	(uint32_t)&SVCall_Handler,
	(uint32_t)&DebugMon_Handler,
	0,
	(uint32_t)&PendSV_Handler,
	(uint32_t)&Systick_Handler,
	(uint32_t)&WWDG_IRQHandler,
	(uint32_t)&EXTI16_PVD_IRQHandler,
	(uint32_t)&EXTI21_TAMP_STAMP_IRQHandler,
	(uint32_t)&EXTI22_RTC_WKUP_IRQHandler,
	(uint32_t)&FLASH_IRQHandler,
	(uint32_t)&RCC_IRQHandler,
	(uint32_t)&EXTI0_IRQHandler,
	(uint32_t)&EXTI1_IRQHandler,
	(uint32_t)&EXTI2_IRQHandler,
	(uint32_t)&EXTI3_IRQHandler,
	(uint32_t)&EXTI4_IRQHandler,
	(uint32_t)&DMA1_Stream0_IRQHandler,
	(uint32_t)&DMA1_Stream1_IRQHandler,
	(uint32_t)&DMA1_Stream2_IRQHandler,
	(uint32_t)&DMA1_Stream3_IRQHandler,
	(uint32_t)&DMA1_Stream4_IRQHandler,
	(uint32_t)&DMA1_Stream5_IRQHandler,
	(uint32_t)&DMA1_Stream6_IRQHandler,
	(uint32_t)&ADC_IRQHandler,
	0,
	0,
	0,
	0,
	(uint32_t)&EXTI9_5_IRQHandler,
	(uint32_t)&TIM1_BRK_TIM9_IRQHandler,
	(uint32_t)&TIM1_UP_TIM10_IRQHandler,
	(uint32_t)&TIM1_TRG_COM_TIM11_IRQHandler,
	(uint32_t)&TIM1_CC_IRQHandler,
	(uint32_t)&TIM2_IRQHandler,
	(uint32_t)&TIM3_IRQHandler,
	(uint32_t)&TIM4_IRQHandler,
	(uint32_t)&I2C1_EV_IRQHandler,
	(uint32_t)&I2C1_ER_IRQHandler,
	(uint32_t)&I2C2_EV_IRQHandler,
	(uint32_t)&I2C2_ER_IRQHandler,
	(uint32_t)&SPI1_IRQHandler,
	(uint32_t)&SPI2_IRQHandler,
	(uint32_t)&USART1_IRQHandler,
	(uint32_t)&USART2_IRQHandler,
	0,
	(uint32_t)&EXTI15_10_IRQHandler,
	(uint32_t)&EXTI17_RTC_Alarm_IRQHandler,
	(uint32_t)&EXTI18_OTG_FS_WKUP_IRQHandler,
	0,
	0,
	0,
	0,
	(uint32_t)&DMA1_Stream7_IRQHandler,
	0,
	(uint32_t)&SDIO_IRQHandler,
	(uint32_t)&TIM5_IRQHandler,
	(uint32_t)&SPI3_IRQHandler,
	0,
	0,
	0,
	0,
	(uint32_t)&DMA2_Stream0_IRQHandler,
	(uint32_t)&DMA2_Stream1_IRQHandler,
	(uint32_t)&DMA2_Stream2_IRQHandler,
	(uint32_t)&DMA2_Stream3_IRQHandler,
	(uint32_t)&DMA2_Stream4_IRQHandler,
	0,
	0,
	0,
	0,
	0,
	0,
	(uint32_t)&OTG_FS_IRQHandler,
	(uint32_t)&DMA2_Stream5_IRQHandler,
	(uint32_t)&DMA2_Stream6_IRQHandler,
	(uint32_t)&DMA2_Stream7_IRQHandler,
	(uint32_t)&USART6_IRQHandler,
	(uint32_t)&I2C3_EV_IRQHandler,
	(uint32_t)&I2C3_ER_IRQHandler,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	(uint32_t)&FPU_IRQHandler,
	0,
	0,
	(uint32_t)&SPI4_IRQHandler
};

/* Reset Handler (entry point) */
void Reset_Handler(void)
{
	/* Initialize .data section - copy data section from FLASH to SRAM */
	// copy .data section to SRAM
	uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;			// size of .data section
	
	// data is stored in bytes so we copy byte-sized chunks
	uint8_t *pSrc = (uint8_t*)&_la_data;							// copying from end of .text section/beginning of .data section in FLASH
	uint8_t *pDst = (uint8_t*)&_sdata;								// copying to start of .data section in RAM
	
	for (uint32_t i = 0; i < size; i++)
	{
		*pDst++ = *pSrc++;											// copy .data section from FLASH to .data section in RAM
	}

	// init the .bss section to zero in SRAM
	size = (uint32_t)&_ebss - (uint32_t)&_sbss;						// size of .bss section
	pDst = (uint8_t*)&_sbss;
	
	for (uint32_t i = 0; i < size; i++)
	{
		*pDst++ = 0;												// initialize .bss section in RAM to 0
	}
	
	// initialize C standard lib
	__libc_init_array();

	// call main()
	main();
}

/* Default Handler */
void Default_Handler(void)
{
	
}