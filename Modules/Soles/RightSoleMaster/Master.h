#include "tm4c123gh6pm.h"
#include <stdint.h>
	#ifndef MASTER_H
 #define MASTER_H
 void initMaster(void);
 void masterSlaveCom(void);
	//------------UART0_OutC har------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void masterOutChar(unsigned char data);

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART3_OutString(unsigned char buffer[]);
//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char masterInChar(void);
//------------UART1_GetName------------
// Output characters to UART1 as they are received from UART1
// Input: none
// Output: none
void UART3_GetName(unsigned char *buffer);
//------------UART1_Echo------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART3_Echo(unsigned char *buffer);
 #endif 