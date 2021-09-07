/* ---------------------------------------------------------------
** File name:		config.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-5-17   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __CONFIG_H_
#define   __CONFIG_H_

/* Includes -----------------------------------------------------*/

/* Macro --------------------------------------------------------*/

//
//  Platform selection
//
#define FPGA_801        (0)
#define ASIC_801        (1)
#define ASIC_801_M      (11)

#define FPGA_802        (2)
#define ASIC_802        (3)

#define PLATFORM        ASIC_801

//
//  SOFT_VERSION
//
#define SDK_VERSION     (0x00020004)//V2.0.4


//
//  Module config
//
#if ( PLATFORM == FPGA_802 ) | ( PLATFORM == ASIC_802 )
#define UART_CHERRY
#endif

//#define SOP16

//#define DEBUG



#define MODULE_TEST
//#define     ENABLE_JTAG


#ifdef  ENABLE_JTAG
#define ENABLE_JTAG_ON_GPIO9_11         // GPIO9.10.11 as jtag
//#define ENABLE_JTAG_ON_GPIO4_6          // GPIO4.5.6 as jtag
#endif


#endif  // #ifndef   __CONFIG_H_
