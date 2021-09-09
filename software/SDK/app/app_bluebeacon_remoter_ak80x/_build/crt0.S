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
.weak   wtd_enable
.weak   error_handler
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
	
	movi      	r2, 201
delay_us_loop:
	.rept 10
	nop
	.endr
	subi      	r2, 1
	zextb      	r2, r2
	cmpnei      	r2, 0
	bt      	delay_us_loop
	
	/*Judge chip version*/
	lrw      	r1, 0x40120180	
	ld.w      	r3, (r1, 0x10)
	cmpnei      	r3, 0
	bt      	jg_loop0
	lrw      	r3, 0x60000000	
	ld.w      	r0, (r3, 0x1c)
	movi      	r2, 0x55
	cmpne      	r0, r2
	bt      	jg_loop1
	br      	ak801m_pro
jg_loop1:
	ld.w      	r1, (r1, 0xc)
	cmpnei      	r1, 0x1c
	bt      	jg_loop2
	st.w      	r2, (r3, 0x1c)
	br      	ak801m_pro
jg_loop2:
	movi      	r2, 0xAA
	st.w      	r2, (r3, 0x1c)
	br      	ak801h_pro
jg_loop0:
	lrw      	r3, 0x60000000	
	ld.w      	r2, (r3, 0x1c)
	movi      	r3, 0x55
	cmpne      	r2, r3
	bt      	ak801h_pro
	br      	ak801m_pro

/*Power on Strategy*/
ak801m_pro:
	#if 0
	movi      	r2, 201
delay_us_loop:
	.rept 10
	nop
	.endr
	subi      	r2, 1
	zextb      	r2, r2
	cmpnei      	r2, 0
	bt      	delay_us_loop
	#endif
	/*Configure the clk*/
	lrw      	r3, 0x40120100
	movi      	r2, 160
	st.w      	r2, (r3, 0x34)

	.rept 10
	nop
	.endr
	
	movi      	r2, 248
	st.w      	r2, (r3, 0x34)
	
	/*TOP_VDD_VCO_REG_0*/
	movi      	r2, 0x12
	st.w      	r2, (r3, 0x1c)
	/*TOP_VDD_VCO_REG_1*/
	movi      	r2, 0x37
	st.w      	r2, (r3, 0x20)
	
	movi      	r2, 101
delay_us_loop1:
	.rept 10
	nop
	.endr
	subi      	r2, 1
	zextb      	r2, r2
	cmpnei      	r2, 0
	bt      	delay_us_loop1
	/*disable watchdog*/
	lrw      	r3, 0x40120100
	movi      	r2, 20
	st.w      	r2, (r3, 0x8c)
	br next_state_flag0

ak801h_pro:
	/*ak801 wdt enable*/
	jbsr wtd_enable
	
next_state_flag0:
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
	lrw         r3,0x60000000
	ld.w        r2, (r3, 0x1c)
	movi        r1, 0xAA
	cmpne       r2, r1
	bt  next_state_flag1
	jbsr error_handler
next_state_flag1:
	br Reset_Handler
	rte
