/*
 * crt0.S
 * 1 define and initial the stack pointer
 * 2 exception handler table
 * 3 call SystemInit
 * 4 go to __main in entry.o
 *
 * Copyright (C) 2016~2017  Hangzhou C-SKY Microsystems Co., Ltd
 * Modify by Jiang Long  on 2016-09-14
 */
// <<< Use Configuration Wizard in Context Menu >>>

.global __initial_sp

// <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:4>
// <i> Config the stack size for the application
#define		StackSize		0x400


.section .csky_stack,"aw",@nobits
.align 2
Stack_Mem:
.space StackSize
__initial_sp:

#ifdef FAST_INTERRUPT_ENABLE
// <o> Fast Stack Size (in Bytes) <0x0-0xFFFFFFFF:4>
// <i> Config the stack size for the fast interrupt's handler
#define		FastStackSize		0 //0x100
FastStack_Mem:
.space FastStackSize
__initial_fast_sp:
#endif

// <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// <i> Config the heap size for the application
#define		HeapSize		0 //0x200

.global __heap_start
.global __heap_end
.section .csky_heap,"aw",@nobits
.align 2
Heap_Mem:
__heap_start:
.space HeapSize
__heap_end:

/*
 * For importing variable or functions from other c or assemble files.
 */
.import __main
.import SystemInit

.import wdt_isr
.import timer0_isr
.import timer1_isr
.import timer2_isr
.import uart_isr
.import gpio_isr
.import rx_isr
.import tx_isr
.import tccr_isr
.import wt_isr
.import lvd_isr
.import core_timer_isr
.import lvd_l_isr
.import lvd_h_isr
.import wtd_enable
.import error_handler

.global wdt_isr
.global timer0_isr
.global timer1_isr
.global timer2_isr
.global uart_isr
.global gpio_isr
.global rx_isr
.global tx_isr
.global tccr_isr
.global wt_isr
.global lvd_isr
.global core_timer_isr
.global lvd_l_isr
.global lvd_h_isr
.global wtd_enable
.global error_handler

.weak   wdt_isr
.weak   timer0_isr
.weak   timer1_isr
.weak   timer2_isr
.weak   uart_isr
.weak   gpio_isr
.weak   rx_isr
.weak   tx_isr
.weak   tccr_isr
.weak   wt_isr
.weak   lvd_isr
.weak   core_timer_isr
.weak   lvd_l_isr
.weak   lvd_h_isr
/*
 * default service routine
 */
.global Reset_Handler
.global Misaligned_Access_Handler
.global Access_Error_Handler
.global Divided_By_Zero_Handler
.global Illegal_Handler
.global Privlege_Violation_Handler
.global Trace_Exection_Handler
.global Breakpoint_Exception_Handler
.global Unrecoverable_Error_Handler
.global Idly4_Error_Handler
.global Auto_INT_Handler
.global Auto_FINT_Handler
.global Reserved_HAI_Handler
.global Reserved_FP_Handler
.global TLB_Ins_Empty_Handler
.global TLB_Data_Empty_Handler
.global Default_handler

.weak Reset_Handler
.weak Misaligned_Access_Handler
.weak Access_Error_Handler
.weak Divided_By_Zero_Handler
.weak Illegal_Handler
.weak Privlege_Violation_Handler
.weak Trace_Exection_Handler
.weak Breakpoint_Exception_Handler
.weak Unrecoverable_Error_Handler
.weak Idly4_Error_Handler
.weak Auto_INT_Handler
.weak Auto_FINT_Handler
.weak Reserved_HAI_Handler
.weak Reserved_FP_Handler
.weak TLB_Ins_Empty_Handler
.weak TLB_Data_Empty_Handler
.weak Default_handler


.export	ckcpu_vsr_table	/* Vector table base address. */
.section .exp_table,"ax",@progbits
/* Vector table space. */
$d:
.align 10
ckcpu_vsr_table:
	.long Reset_Handler
	.long Misaligned_Access_Handler
	.long Access_Error_Handler
	.long Divided_By_Zero_Handler
	.long Illegal_Handler
	.long Privlege_Violation_Handler
	.long Trace_Exection_Handler
	.long Breakpoint_Exception_Handler
	.long Unrecoverable_Error_Handler
	.long Idly4_Error_Handler
	.long Auto_INT_Handler
	.long Auto_FINT_Handler
	.long Reserved_HAI_Handler
	.long Reserved_FP_Handler
	.long TLB_Ins_Empty_Handler
	.long TLB_Data_Empty_Handler
.rept 16	
	.long empty_handler
.endr
	.long wdt_isr
	.long timer0_isr
	.long timer1_isr
	.long timer2_isr
	.long uart_isr
	.long gpio_isr
	.long rx_isr
	.long tx_isr
	.long tccr_isr
	.long wt_isr
	.long lvd_isr
	.long core_timer_isr
	.long lvd_l_isr
	.long lvd_h_isr
.rept 20
	.long Default_handler
.endr
$t:
/* The ckcpu startup codes. */
.text
.align 2
	/*
	 * This is the codes first entry point. This is where it all begins...
	 */
Reset_Handler:
	/*
	 * Init psr value, enable exception, disable interrupt and fast interrupt.
	 * psr = 0x80000100
	 */
	lrw		a3,	0x80000100
	mtcr	a3, psr

	/*Configure the watchdog time*/
	jbsr wtd_enable

	/*
	 * Setup initial vector base table for interrupts and exceptions
	 */
	lrw 	a3, ckcpu_vsr_table
	mtcr	a3, vbr
	
	/* Initialize the normal stack pointer from the linker definition. */
	lrw 	a3, 0x20000800 //__initial_sp	//0x20001000
	mov		sp, a3

#ifdef FAST_INTERRUPT_ENABLE
	/* Initialize the fast interrupt stack pointer . */
	psrset	af
	lrw		r6, __initial_fast_sp
	mov		sp, r6
	psrclr	af
#endif

	lrw a0,	SystemInit
	jsr a0
	lrw a0,	__main
	jsr a0
	
	/* Should never get here. */
1:
	br 1b
Misaligned_Access_Handler:
Access_Error_Handler:
Divided_By_Zero_Handler:
Illegal_Handler:
Privlege_Violation_Handler:
Trace_Exection_Handler:
Breakpoint_Exception_Handler:
Unrecoverable_Error_Handler:
Idly4_Error_Handler:
Auto_INT_Handler:
Auto_FINT_Handler:
Reserved_HAI_Handler:
Reserved_FP_Handler:
TLB_Ins_Empty_Handler:
TLB_Data_Empty_Handler:
empty_handler:
Default_handler:
	//br Reset_Handler
	lrw a0,	error_handler
	jsr a0
	lrw a0,	Reset_Handler
	jsr a0
	rte
