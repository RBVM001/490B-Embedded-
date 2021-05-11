#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <stdint.h>
 #ifndef SENDER_H
 #define SENDER_H
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

//------------UART1_Init------------
// Initialize the UART for 115200 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, No parity, one stop bit, FIFOs enabled
// Input: none
// Output: none
void initSender(void);

//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART1_InChar(void);

//------------UART0_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(unsigned char data);

//------------UART1_Echo------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART1_Echo(unsigned char *buffer);

//-----------------------UART_StepOut-----------------------
// Output a 32-bit number in ASCII unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits, one space after, CR termination
void UART1_StepOut(unsigned long n);


//------------UART1_GetName------------
// Output characters to UART1 as they are received from UART1
// Input: none
// Output: none
void senderGetName(unsigned char *buffer);
void sendAxis(unsigned long x, unsigned long y,unsigned long z);
void sendAvgAxis(unsigned long x, unsigned long y, unsigned z);

#endif
