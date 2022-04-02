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

/**
 * @brief  Version of the current SDK.
 */
#define SDK_VERSION     (0x00020013)    //V2.0.13
#define SDK_DATE        (0x20220120)    //2022.01.0

/**
 * @brief  Enable the function of 'print'.
 */
//  #define DEBUG

/**
 * @brief  The test cases of SDK.
 */
//  #define MODULE_TEST

/**
 * @brief  Enable the function of 'JTAG'.
 */
//#define     ENABLE_JTAG

#ifdef  ENABLE_JTAG
#define ENABLE_JTAG_ON_GPIO9_11             // GPIO 9.10.11 as jtag
//#define ENABLE_JTAG_ON_GPIO4_6            // GPIO 4.5.6 as jtag
#endif

#endif  // #ifndef   __CONFIG_H_
