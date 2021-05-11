#include "tm4c123gh6pm.h"
#include <stdint.h>
 #ifndef SLAVE_H
 #define SLAVE_H
 void initSlave(void);
 void signalSlaveMaster(void);
	//------------UART0_OutC har------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void slaveOutChar(unsigned char data);

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void slaveOutString(unsigned char buffer[]);
//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char slaveInChar(void);
//------------UART1_GetName------------
// Output characters to UART1 as they are received from UART1
// Input: none
// Output: none
void slaveGetName(unsigned char *buffer);
//------------UART1_Echo------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void slaveEcho(unsigned char *buffer);
unsigned receiveNameSlave();
 #endif 