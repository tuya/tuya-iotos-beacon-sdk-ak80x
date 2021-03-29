/* ---------------------------------------------------------------
** File name:		type.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**	Date 		Version		Action				Author
** ---------------------------------------------------------------
**	2019-1-12 	1.0			Created				Yan Lei
**
** ---------------------------------------------------------------
*/

#ifndef   __TYPE_H_
#define   __TYPE_H_

#ifdef __cplusplus
extern "C" {    // allow C++ to use these headers
#endif

/* Includes -----------------------------------------------------*/

/* Macro --------------------------------------------------------*/

#ifndef NULL
#define NULL   (0)
#endif      // NULL
#ifndef null
#define null   NULL
#endif      // null

#ifndef TRUE
#define TRUE    (1)
#endif      // TRUE
#ifndef true
#define true    TRUE
#endif      // true

#ifndef FALSE
#define FALSE   (0)
#endif      // FALSE
#ifndef false
#define false   FALSE
#endif      // false

#ifndef EOF
#define EOF     (-1)
#endif      // EOF

#ifndef ON
#define ON      (1)
#endif      // ON

#ifndef OFF
#define OFF     (0)
#endif      // OFF

#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif


typedef enum error_type
{
    ERR_NO_ERROR        = 0,
    ERR_NOT_SUPPORTED   = 1,
    ERR_WRONG_PARA      = 2
} error_t;

#if 1

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int   uint32_t;

typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int     int32_t;

#ifndef __STDBOOL_H__
//typedef unsigned char bool;
typedef unsigned char BOOL;
#endif

#else

#define uint8_t     unsigned char
#define uint16_t    unsigned short
#define uint32_t    unsigned long

#define int8_t      signed char
#define int16_t     signed short
#define int32_t     signed long

#endif


#ifdef __cplusplus
}
#endif

#endif  // __TYPE_H_

