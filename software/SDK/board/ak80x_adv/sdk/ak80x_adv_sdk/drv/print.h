/* ---------------------------------------------------------------
** File name:		print.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**	Date        Version     Action              Author
** ---------------------------------------------------------------
**	2019-1-12   1.0         Created             Yan Lei
**
** ---------------------------------------------------------------
*/

#ifndef   __PRINT_H_
#define   __PRINT_H_


#ifdef __cplusplus
extern "C" {    // allow C++ to use these headers
#endif

/* Includes -----------------------------------------------------*/
#include "config.h"

/* Macro --------------------------------------------------------*/

#ifdef  DEBUG

#define print   myPrintf
void myPrintf(char *s, ...);

#else   // DEBUG

#define print   /\
/myPrintf

#endif


/* Functions prototype ------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // __PRINT_H_

