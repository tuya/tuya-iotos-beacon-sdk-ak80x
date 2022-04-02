/* ---------------------------------------------------------------
** File name:		pwm.c
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


/* Includes -----------------------------------------------------*/

#include "config.h"

#ifdef   DEBUG

#include <stdio.h>
#include <stdarg.h>
#include "uart.h"


/* Macro --------------------------------------------------------*/

//#define putchar     uart_putc

#ifdef  fputc
#undef  fputc
#define fputc     uart_putc
#endif

#define putchar uart_putc

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/


void myPrintf(char *s, ...);
void printNum(unsigned long num, int base);
void printDeci(int dec);
void printOct(unsigned oct);
void printHex(unsigned hex);
void printAddr(unsigned long addr);
void printStr(char *str);
void printFloat(double f);


void myPrintf(char *s, ...)
{
    int i = 0;

    va_list va_ptr;
    va_start(va_ptr, s);

    while (s[i] != '\0')
    {
		if (s[i] != '%')
		{
    	    putchar(s[i++]);
			continue;
		}
		
		switch (s[++i])
		{
			case 'd': printDeci(va_arg(va_ptr,int));           
			  		  break; 
		    case 'o': printOct(va_arg(va_ptr,unsigned int));  
			  		  break;
		    case 'x': printHex(va_arg(va_ptr,unsigned int));  
			  		  break;
		    case 'c': putchar(va_arg(va_ptr,int));            
			  		  break;
		    case 'p': printAddr(va_arg(va_ptr,unsigned long));
			  		  break;
		    case 'f': printFloat(va_arg(va_ptr,double));      
			  		  break;
		    case 's': printStr(va_arg(va_ptr,char *));
					  break;
			default : break;
		}

		i++;
    }

    va_end(va_ptr);
}

void printNum(unsigned long num, int base)
{
	if (num == 0)
    {
        return;
    }
    
	printNum(num/base, base);

    putchar("0123456789abcdef"[num%base]);
}


void printDeci(int dec)
{
	if (dec < 0)
    {
        putchar('-');
		dec = -dec;
    }

    if (dec == 0)
    {
        putchar('0');
		return;
    }
    else
    {
        printNum(dec, 10);
    }
}

void printOct(unsigned oct)
{
    if (oct == 0)
    {
		putchar('0');
		return;
    }
    else
    {
        printNum(oct,8);
    }
}

void printHex(unsigned hex)
{
    if (hex == 0)
    {
        putchar('0');
		return;
    }
    else
    {
        printNum(hex,16);
    }
}

void printAddr(unsigned long addr)
{
	putchar('0');
    putchar('x');

    printNum(addr, 16);
}

void printStr(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        putchar(str[i++]);
    }
}

void printFloat(double f)
{
    int temp;

    temp = (int)f;
    printNum(temp,10);
	
    putchar('.');

    f -= temp;
    if (f == 0)
    {
		for (temp = 0; temp < 6; temp++)
		{
		    putchar('0');
		}
		return;
    }
    else
    {
        temp = (int)(f*1000000);
        printNum(temp,10);
    }
}

int scanf(const char *fmt,...)
{
    int i = 0;
    unsigned char c;
    va_list args;
    char buffer[10];
    unsigned char k = 0;
        
#if 1
    while(1)
    {
        // Recieve characters from UART
        k = uart_get_rx_status();
        if ( k )
        {
                c = uart_getc();
                uart_putc(c);
                if((c == 0x0d) || (c == 0x0a))
                {
                    buffer[i] = ' ';
                    break;
                }
                else
                {
                    buffer[i++] = c;
                }
        }
    }
#else
    while(1)
    {
        c = uart_getc();
        uart_putc(c);
        if((c == 0x0d) || (c == 0x0a))
        {
            buffer[i] = ' ';
            break;
        }else{
            buffer[i++] = c;
        }
    }
#endif

    va_start(args,fmt);
    i = vsscanf(buffer,fmt,args);
    va_end(args);

    uart_putc('\r');
    uart_putc('\n');

    return i;
}

#else

void myPrintf(char *s, ...)
{
    
}

#endif	//#ifdef	DEBUG

